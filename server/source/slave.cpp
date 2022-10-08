#include "slave.h"

#include "../../FunEngine2D/core/include/tools/debugger.h"
#include "../../common/include/texel_batch.h"

// namespace {
    // bool set_texel(space::canvas_t& canvas) {
    //     return true;
    // }

    // void send_texel(fun::server_t& server, fun::vec2i_t pos, fun::rgb_t color) {
    //     // todo: change sending method
    //     server.send_all(
    //         "s " +
    //         std::to_string(pos.x) + " " +
    //         std::to_string(pos.y) + " " +
    //         std::to_string(color.r) + " " +
    //         std::to_string(color.g) + " " +
    //         std::to_string(color.b)
    //     );
    // }

    // void send_chunk(fun::server_t& server, space::canvas_t& canvas, fun::vec2_t <space::chunk_int_t> pos, sf::TcpSocket* sender) {
    //     server.send_all(
    //         space::chunk::encode(
    //             {pos.x, pos.y},
    //             canvas.get_chunk(pos.x, pos.y)->get_data()
    //         ), sender);
    // }
// }

// s - set texel color (x, y, r, g, b)
// f - fetch chunk data (x, y)
// b - set chunk subscribtion range
// cn - set clientname


namespace {
    space::state_t* state;
    fun::network::threadsafe_packet_storage_t* packet_storage;

    void process(const fun::str_t& cmd_str, sf::TcpSocket* sender) {
        space::server_cmd_t cmd_type = (space::server_cmd_t)cmd_str[0];

        // fun::debugger::push_msg("received bytes: " + std::to_string(cmd_str.size()));

        switch(cmd_type) {
            case space::server_cmd_t::receive_batch: {
                space::texel_batch_t batch;
                space::texel_batch_t new_batch;

                batch.from_cmd(cmd_str);

                state->statistics.texels_placed += batch.get_total_texels();
                state->clients_data[sender].texels_placed += batch.get_total_texels();

                auto& data = batch.get_data();

                {
                    std::unique_lock canvas_unique_lock(state->canvas.key);

                    for (auto& [chunk_pos, _] : data) {
                        state->canvas.init_chunk(chunk_pos);
                    }
                }
 
                {
                    std::shared_lock canvas_shared_lock(state->canvas.key);

                    for (auto& [chunk_pos, texels] : data) {
                        std::lock_guard chunk_lock(state->canvas.get_chunk(chunk_pos)->key);

                        for (auto texel : *texels) {
                            state->canvas.set_color(chunk_pos, texel.pos, texel.color);
                            new_batch.add_texel(chunk_pos, texel.pos, state->canvas.get_color(chunk_pos, texel.pos));
                        }
                    }
                }

                state->server.send_all(new_batch.to_cmd());

                break;
            }

            case space::server_cmd_t::request_chunk: {
                space::chunk_pos_t chunk_pos = *(space::chunk_pos_t*)&cmd_str[1];

                fun::str_t chunk_str;

                bool should_send = false;

                {
                    std::shared_lock canvas_shared_lock(state->canvas.key);

                    if (state->canvas.has_chunk(chunk_pos)) {
                        space::chunk_t* chunk = state->canvas.get_chunk(chunk_pos);

                        should_send = true;

                        std::lock_guard chunk_lock(chunk->key);
                        
                        chunk_str = space::chunk::encode(chunk_pos, chunk->get_colors());
                    }
                }

                if (should_send) {
                    chunk_str.insert(chunk_str.begin(), (char)space::server_cmd_t::receive_chunk);

                    state->server.send(chunk_str, sender);
                }

                break;
            }

            case space::server_cmd_t::request_all_chunks: {
                fun::str_t chunks_str(1, (char)space::server_cmd_t::receive_all_chunks);

                {
                    std::shared_lock canvas_shared_lock(state->canvas.key);

                    auto& chunks = state->canvas.get_chunks();

                    for (auto& [chunk_pos, chunk] : chunks) {
                        std::lock_guard chunk_lock(chunk->key);

                        chunks_str += space::chunk::encode(chunk_pos, chunk->get_colors());
                    }
                }
                
                if (chunks_str.size() > 1) {
                    state->server.send(chunks_str, sender);
                }

                break;
            }

            case space::server_cmd_t::receive_message: {
                fun::command_t received_cmd(cmd_str);

                fun::command_t command;
                command.set_command(std::string(1, (char)space::server_cmd_t::receive_message));

                if (received_cmd.has_key("target")) {
                    std::string target = received_cmd.get_val("target");

                    for (auto& [client, data] : state->clients_data) {
                        if (data.name == target) {
                            command.add_arg(received_cmd.get_arg(0));
                            command.add_key_val("gossip", state->clients_data[sender].name);

                            state->server.send(command.build(), client);

                            break;
                        }
                    }
                } else {
                    command.add_arg(received_cmd.get_arg(0));

                    state->server.send_all(command.build());
                }

                break;
            }
        }
    }

    fun::async worker(uint32_t thread_id) {
        while (true) {
            fun::network::packet_t packet = packet_storage->read();

            process(packet.data, packet.sender);
        }
    }

    void backup_canvas() {
        std::shared_lock canvas_shared_lock(state->canvas.key);
        
        for (auto& [chunk_pos, chunk] : state->canvas.get_chunks()) {
            std::ofstream save_file;

            {
                std::lock_guard chunk_lock(chunk->key);

                if (chunk->updated) {
                    save_file.open("data/chunks/" + std::to_string(chunk_pos.x) + "_" + std::to_string(chunk_pos.y) + ".chunk", std::ios::binary);

                    save_file << space::chunk::encode(chunk_pos, chunk->get_colors());

                    chunk->updated = false;
                }
            }

            save_file.close();
        }
    }

    fun::async backup_state() {
        static constexpr uint32_t backup_interval = 2;

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(backup_interval));

            backup_canvas();
        }
    }

    void load_chunks() {
        // this happens before running threads so no need to lock canvas

        uint32_t chunk_count = 0;

        for (auto const& chunk_file : std::filesystem::directory_iterator("data/chunks")) {
            std::ifstream chunk_file_stream(chunk_file.path().string(), std::ios::binary);

            fun::str_t chunk_str;
            chunk_file_stream >> chunk_str;

            space::chunk_pos_t chunk_pos = space::chunk::decode_position(chunk_str);

            state->canvas.init_chunk(chunk_pos);
            space::chunk::decode_colors(chunk_str, state->canvas.get_chunk(chunk_pos)->get_colors());

            chunk_count++;
        }

        fun::debugger::push_msg("loaded " + std::to_string(chunk_count) + " chunks", "save");
    }

    void setup_directories() {
        if (!std::filesystem::is_directory("data")) {
            std::filesystem::create_directory("data");
        }
        
        if (!std::filesystem::is_directory("data/chunks")) {
            std::filesystem::create_directory("data/chunks");
        }
    }
}

void space::slave::run(uint32_t thread_count, state_t* state) {
    ::state = state;
    ::packet_storage = &state->server.get_packet_storage();

    ::setup_directories();

    ::load_chunks();

    for (uint32_t i = 0; i < thread_count; i++) {
        std::thread(::worker, i).detach();
    }

    std::thread(::backup_state).detach();
}