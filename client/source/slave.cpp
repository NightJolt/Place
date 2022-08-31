#include "slave.h"

#include "../../FunEngine2D/core/include/tools/debugger.h"

// t - teleport to coordinates
// s - set texel color
// g - get texel color
// z - zoom
// e - export
// msg - message

void space::slave::set_clientname(state_t& state, const std::string& clientname) {
    fun::command_t command;

    command.set_command("cn");
    command.set_command(clientname);

    state.client.send(command.build());
}

void space::slave::send_texel(state_t& state, grid_pos_t pos, fun::rgb_t color) {
    if (state.canvas.get_color(pos) == color) return;
    
    state.canvas.set_color(pos, state.tool.color);
    state.batch.add_texel(pos, state.tool.color);
}

void space::slave::request_chunk(state_t& state, chunk_pos_t chunk_pos) {
    fun::str_t request;
    request.resize(1 + sizeof grid_pos_t);

    request[0] = space::request_chunk;
    *(chunk_pos_t*)(&request[1]) = chunk_pos;

    state.client.send(request);
}

void space::slave::send_message(state_t& state, const std::string& msg) {
    fun::command_t command;

    command.set_command("msg");
    command.add_arg(msg);

    state.client.send(command.build());
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
    }
}