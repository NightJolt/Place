#include "slave.h"

// t - teleport to coordinates
// s - set texel color
// g - get texel color
// z - zoom
// e - export

void space::slave::send_texel(fun::client_t& client, canvas_t& canvas, fun::vec2_t <texel_pos_t> pos, fun::rgb_t color) {
    fun::command_t command;

    command.set_command("s");

    command.add_arg(std::to_string(pos.x));
    command.add_arg(std::to_string(pos.y));
    
    command.add_arg(std::to_string(color.r));
    command.add_arg(std::to_string(color.g));
    command.add_arg(std::to_string(color.b));

    client.send(command.build());
}