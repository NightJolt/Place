#include "slave.h"

#include "../../FunEngine2D/core/include/tools/debugger.h"

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

    void process(const std::string& cmd_str, sf::TcpSocket* sender) {
        fun::command_t command_parser(cmd_str);
        const std::string& command = command_parser.get_command();

        if (command == "f") {
            // ::send_chunk(server, canvas, { (space::chunk_int_t)std::stoi(command_parser.get_arg(0)), (space::chunk_int_t)std::stoi(command_parser.get_arg(1)) }, sender);
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

            state->canvas.set_color({ pos.x, pos.y }, color);

            state->statistics.texels_placed++;

            // ::send_texel(server, pos, color);
            state->server.send_all(cmd_str, sender);
        } else if (command == "cn") {
            state->client_datas[sender].name = command_parser.get_arg(0);
        }
    }

    fun::async worker(uint32_t thread_id) {
        while (true) {
            fun::network::packet_t packet = packet_storage->read();

            fun::debugger::push_msg("processing " + std::to_string(packet.data.length()) + " bytes on thead " + std::to_string(thread_id) + " : " + packet.data, "slave");

            process(packet.data, packet.sender);
        }
    }
}


void space::slave::run(uint32_t thread_count, state_t* state) {
    ::state = state;
    ::packet_storage = &state->server.get_packet_storage();

    for (uint32_t i = 0; i < thread_count; i++) {
        std::thread(worker, i).detach();
    }
}