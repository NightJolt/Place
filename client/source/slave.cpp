#include "slave.h"

#include "../../FunEngine2D/core/include/tools/debugger.h"

// t - teleport to coordinates
// s - set texel color
// g - get texel color
// z - zoom
// e - export
// msg - message

void space::slave::send_message(state_t& state, const std::string& message) {
    fun::command_t command;

    command.set_command(std::string(1, (char)server_cmd_t::receive_message));
    command.add_arg(message);

    state.client.send(command.build());
}

void space::slave::process_command(state_t& state, std::string& cmd_str) {
    fun::command_t cmd_obj(cmd_str);
    const std::string& cmd = cmd_obj.get_command();

    if (cmd == "gossip") {
        if (cmd_obj.get_args().size() < 2) return;

        fun::command_t res_cmd_obj;

        res_cmd_obj.set_command(std::string(1, (char)server_cmd_t::receive_message));
        res_cmd_obj.add_key_val("target", cmd_obj.get_arg(0));

        std::string text;

        for (int i = 1; i < cmd_obj.get_args().size(); i++) {
            if (i != 1) {
                text.append(" ");
            }
            
            text.append(cmd_obj.get_arg(i));
        }

        res_cmd_obj.add_arg(text);

        state.client.send(res_cmd_obj.build());
    } else if (cmd == "amogusify" || cmd == "sussify") {
        // mugusfy
    }
}

void space::slave::send_texel(state_t& state, grid_pos_t pos, fun::rgb_t color) {
    if (state.canvas.get_color(pos) == color) return;
    
    state.canvas.set_color(pos, color);
    state.batch.add_texel(pos, color);
}

void space::slave::request_chunk(state_t& state, chunk_pos_t chunk_pos) {
    fun::str_t request;
    request.resize(1 + sizeof grid_pos_t);

    request[0] = (char)space::server_cmd_t::request_chunk;
    *(chunk_pos_t*)(&request[1]) = chunk_pos;

    state.client.send(request);
}

void space::slave::request_all_chunks(state_t& state) {
    state.client.send(std::string(1, (char)space::server_cmd_t::request_all_chunks));
}

void space::slave::step(state_t& state, float delta_time) {
    {
        if (state.batch_cooldown > 0) state.batch_cooldown -= delta_time;
        if (state.batch.get_total_texels() == 0) state.batch_cooldown = state.batch_send_interval;

        if (state.batch_cooldown <= 0 || state.batch_max_texels <= state.batch.get_total_texels()) {
            state.batch_cooldown = state.batch_send_interval;

            state.client.send(state.batch.to_cmd());
            state.batch.clear();
        }
    }

    {
        state.client.receive();
    
        auto& packet_storage = state.client.get_packets();

        if (!packet_storage.empty()) {
            fun::network::packet_t packet = packet_storage.read();

            space::slave::process(state, packet);
        }
    }
}
    
void space::slave::process(state_t& state, const fun::network::packet_t& packet) {
    space::server_cmd_t cmd_type = (space::server_cmd_t)packet.data[0];

    fun::debugger::push_msg("received bytes: " + std::to_string(packet.data.size()));

    switch(cmd_type) {
        case space::server_cmd_t::receive_batch: {
            space::texel_batch_t batch;

            batch.from_cmd(packet.data);

            auto& data = batch.get_data();

            for (auto& [chunk_pos, texels] : data) {
                for (auto texel : *texels) {
                    state.canvas.set_color(chunk_pos, texel.pos, texel.color);
                }
            }

            break;
        }

        case space::server_cmd_t::receive_chunk: {
            const fun::str_t& data = packet.data.substr(1);

            chunk_pos_t chunk_pos = space::chunk::decode_position(data);

            std::vector <fun::rgb_t> colors;
            colors.resize(space::chunk_volume);

            space::chunk::decode_colors(data, &colors[0]);

            state.canvas.get_chunk(chunk_pos)->set_colors(colors);

            break;
        }

        case space::server_cmd_t::receive_all_chunks: {
            uint32_t ind = 1;

            std::vector <fun::rgb_t> colors;
            colors.resize(space::chunk_volume);

            while (ind < packet.data.size()) {
                chunk_pos_t chunk_pos = space::chunk::decode_position(packet.data, ind);

                space::chunk::decode_colors(packet.data, &colors[0], ind);
                state.canvas.get_chunk(chunk_pos)->set_colors(colors);

                ind += sizeof chunk_pos_t + space::chunk_volume * sizeof fun::rgb_t;
            }

            break;
        }

        case space::server_cmd_t::receive_message: {
            fun::command_t command(packet.data);

            if (command.has_key("gossip")) {
                state.messages.emplace_back(command.get_arg(0), state_t::message_type_t::client_gossip);
            } else {
                state.messages.emplace_back(command.get_arg(0), state_t::message_type_t::client_message);
            }

            break;
        }
    }
}