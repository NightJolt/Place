#include "slave.h"

// t - teleport to coordinates
// s - set texel color
// g - get texel color
// z - zoom
// e - export

void space::slave::send_texel(fun::client_t& client, canvas_t& canvas, fun::vec2_t <grid_pos_t> pos, fun::rgb_t color) {
    if (canvas.get_color(pos) == color) return;

    canvas.set_color(pos, color);

    fun::command_t command;

    command.set_command("s");

    command.add_arg(std::to_string(pos.x));
    command.add_arg(std::to_string(pos.y));
    
    command.add_arg(std::to_string(color.r));
    command.add_arg(std::to_string(color.g));
    command.add_arg(std::to_string(color.b));

    client.send(command.build());
}
    
void space::slave::process(fun::client_t& client, canvas_t& canvas, const fun::command_t& command_parser) {
    const std::string& command = command_parser.get_command();

    if (command == "s") {
        fun::vec2i_t pos = {
            std::stoi(command_parser.get_arg(0)),
            std::stoi(command_parser.get_arg(1))
        };

        fun::rgb_t color = {
            (uint8_t)std::stoi(command_parser.get_arg(2)),
            (uint8_t)std::stoi(command_parser.get_arg(3)),
            (uint8_t)std::stoi(command_parser.get_arg(4))
        };

        canvas.set_color({ pos.x, pos.y }, color);
    }
}