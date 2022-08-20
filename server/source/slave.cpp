#include "slave.h"

namespace {
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

    // void send_chunk(fun::server_t& server, space::canvas_t& canvas, fun::vec2_t <space::chunk_pos_t> pos, sf::TcpSocket* sender) {
    //     server.send_all(
    //         space::chunk::encode(
    //             {pos.x, pos.y},
    //             canvas.get_chunk(pos.x, pos.y)->get_data()
    //         ), sender);
    // }
}

// s - set texel color (x, y, r, g, b)
// f - fetch chunk data (x, y)
// b - set chunk subscribtion range
// cn - set clientname

void space::slave::process(state_t& state, const fun::command_t& command_parser, sf::TcpSocket* sender) {
    const std::string& command = command_parser.get_command();

    // println(command_parser.build());

    if (command == "f") {
        // ::send_chunk(server, canvas, { (space::chunk_pos_t)std::stoi(command_parser.get_arg(0)), (space::chunk_pos_t)std::stoi(command_parser.get_arg(1)) }, sender);
    } else if (command == "s") {
        fun::vec2i_t pos = {
            std::stoi(command_parser.get_arg(0)),
            std::stoi(command_parser.get_arg(1))
        };

        fun::rgb_t color = {
            (uint8_t)std::stoi(command_parser.get_arg(2)),
            (uint8_t)std::stoi(command_parser.get_arg(3)),
            (uint8_t)std::stoi(command_parser.get_arg(4))
        };

        state.canvas.set_color({ pos.x, pos.y }, color);

        state.statistics.texels_placed++;

        // ::send_texel(server, pos, color);
        state.server.send_all(command_parser.build());
    } else if (command == "cn") {
        state.client_datas[sender].name = command_parser.get_arg(0);
    }
}