#include "../FunEngine2D/core/include/globals.h"
#include "../FunEngine2D/core/include/vec2.h"
#include "../FunEngine2D/core/include/_time.h"
#include "../FunEngine2D/core/include/render/window_manager.h"
#include "../FunEngine2D/core/include/networking/client.h"
#include "../FunEngine2D/core/include/networking/server.h"
#include "../FunEngine2D/core/include/input.h"

#include "../common/include/space.h"

// t - teleport to coordinates
// s - set pixel color
// g - get pixel color
// z - zoom
// e - export

int main () {
    fun::winmgr::init(fun::winmgr::window_data_t("Place Client"));
    auto* window = fun::winmgr::main_window;

    fun::client_t client;
    if (!client.connect("localhost", 8001)) exit(69);
    
    while (window->render.isOpen()) {
        fun::time::recalculate();
        fun::winmgr::update();
        fun::input::listen();

        if (fun::input::pressed(sf::Keyboard::Space)) client.send("s 1 2 255 150 0");

        client.receive();
        
        auto& packet_storage = client.get_packets();

        if (!packet_storage.empty()) {
            fun::packet_storage_t::packet_t packet = packet_storage.read();
            fun::command_t command_parser = fun::command_t(packet.data);
            const std::string& command = command_parser.get_command();

            if (command == "s") {
                fun::vec2_t pos = {
                    std::stoi(command_parser.get_arg(0)),
                    std::stoi(command_parser.get_arg(1))
                };

                fun::rgb_t color = {
                    (uint8_t)std::stoi(command_parser.get_arg(2)),
                    (uint8_t)std::stoi(command_parser.get_arg(3)),
                    (uint8_t)std::stoi(command_parser.get_arg(4))
                };
                
                // ! smth here
            }
        }

        window->display(sf::Color::Black);
    }

    client.disconnect();

    fun::winmgr::close();

    return 0;
};