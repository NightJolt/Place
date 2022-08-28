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

    // void process(const fun::str_t& cmd_str, sf::TcpSocket* sender) {
    //     fun::command_t command_parser(cmd_str);
    //     const fun::str_t& command = command_parser.get_command();

    //     if (command == "f") {
    //         // ::send_chunk(server, canvas, { (space::chunk_int_t)std::stoi(command_parser.get_arg(0)), (space::chunk_int_t)std::stoi(command_parser.get_arg(1)) }, sender);
    //     } else if (command == "s") {
    //         fun::vec2i_t pos = {
    //             std::stoi(command_parser.get_arg(0)),
    //             std::stoi(command_parser.get_arg(1))
    //         };

    //         fun::rgb_t color = {
    //             (uint8_t)std::stoi(command_parser.get_arg(2)),
    //             (uint8_t)std::stoi(command_parser.get_arg(3)),
    //             (uint8_t)std::stoi(command_parser.get_arg(4))
    //         };

    //         state->canvas.set_color({ pos.x, pos.y }, color);

    //         state->statistics.texels_placed++;

    //         // ::send_texel(server, pos, color);
    //         state->server.send_all(cmd_str, sender);
    //     } else if (command == "cn") {
    //         state->client_datas[sender].name = command_parser.get_arg(0);
    //     }
    // }

    void process(const fun::str_t& cmd_str, sf::TcpSocket* sender) {
        space::server_cmd_t cmd_type = (space::server_cmd_t)cmd_str[0];

        switch(cmd_type) {
            case space::server_cmd_t::send_batch: {
                space::texel_batch_t batch;
                space::texel_batch_t new_batch;

                batch.from_str(cmd_str);

                state->statistics.texels_placed += batch.get_total_texels();

                fun::debugger::push_msg("received batch of " + std::to_string(batch.get_total_texels()) + " texels");

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

                state->server.send_all(new_batch.to_str());

                break;
            }
        }
    }

    fun::async worker(uint32_t thread_id) {
        while (true) {
            fun::network::packet_t packet = packet_storage->read();

            fun::debugger::push_msg("processing " + std::to_string(packet.data.length()) + " bytes on thead " + std::to_string(thread_id) + " : " + packet.data, "slave");

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

                    save_file.write((char*)&chunk_pos, sizeof space::chunk_pos_t);
                    save_file << chunk->to_str();

                    chunk->updated = false;
                }
            }

            save_file.close();
        }
    }

    fun::async backup_state() {
        static constexpr uint32_t backup_interval = 2;

        if (!std::filesystem::is_directory("data/chunks")) std::filesystem::create_directories("data/chunks");

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(backup_interval));

            backup_canvas();
        }
    }
}


void space::slave::run(uint32_t thread_count, state_t* state) {
    ::state = state;
    ::packet_storage = &state->server.get_packet_storage();

    for (uint32_t i = 0; i < thread_count; i++) {
        std::thread(worker, i).detach();
    }

    std::thread(backup_state).detach();
}