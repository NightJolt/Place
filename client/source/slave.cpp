#include "slave.h"

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

void space::slave::send_texel(state_t& state, fun::vec2_t <grid_pos_t> pos, fun::rgb_t color) {
    if (state.canvas.get_color(pos) == color) return;

    state.canvas.set_color(pos, color);

    fun::command_t command;

    command.set_command("s");

    command.add_arg(std::to_string(pos.x));
    command.add_arg(std::to_string(pos.y));
    
    command.add_arg(std::to_string(color.r));
    command.add_arg(std::to_string(color.g));
    command.add_arg(std::to_string(color.b));

    state.client.send(command.build());
}

void space::slave::send_message(state_t& state, const std::string& msg) {
    fun::command_t command;

    command.set_command("msg");
    command.add_arg(msg);

    state.client.send(command.build());
}
    
void space::slave::process_command(state_t& state, const std::string& cmd_str) {
    const fun::command_t command_parser = fun::command_t(cmd_str);

    process_command(state, command_parser);
}

void space::slave::process_command(state_t& state, const fun::command_t& command_parser) {
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

        state.canvas.set_color({ pos.x, pos.y }, color);
    }
}