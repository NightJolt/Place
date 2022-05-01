#include "slave.h"

namespace {
    uint32_t texels_set = 0;

    bool set_texel(space::canvas_t& canvas) {
        return true;
    }

    void send_texel(fun::server_t& server, fun::vec2i_t pos, fun::rgb_t color) {
        // todo: change sending method
        server.send_all(
            "s " +
            std::to_string(pos.x) + " " +
            std::to_string(pos.y) + " " +
            std::to_string(color.r) + " " +
            std::to_string(color.g) + " " +
            std::to_string(color.b)
        );
    }

    void send_chunk(fun::server_t& server, space::canvas_t& canvas, fun::vec2_t <space::chunk_pos_t> pos, sf::TcpSocket* sender) {
        server.send_all(
            space::chunk::encode(
                pos.x,
                pos.y,
                space::chunk_size_x,
                space::chunk_size_y,
                canvas.get_chunk(pos.x, pos.y)->get_data()
            ), sender);
    }
}

// s - set pixel color (x, y, r, g, b)
// f - fetch chunk data (x, y)
// b - set chunk subscribtion range

void space::slave::process(fun::server_t& server, canvas_t& canvas, const fun::command_t& command_parser, sf::TcpSocket* sender) {
    const std::string& command = command_parser.get_command();

    if (command == "f") {
        ::send_chunk(server, canvas, { (space::chunk_pos_t)std::stoi(command_parser.get_arg(0)), (space::chunk_pos_t)std::stoi(command_parser.get_arg(1)) }, sender);
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

        canvas.set_color(pos.x, pos.y, color);

        ::texels_set++;

        ::send_texel(server, pos, color);
    }
}

uint32_t space::slave::get_texels_set() {
    return ::texels_set;
}