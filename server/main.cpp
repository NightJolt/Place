#include "../FunEngine2D/core/include/globals.h"
#include "../FunEngine2D/core/include/vec2.h"
#include "../FunEngine2D/core/include/color.h"
#include "../FunEngine2D/core/include/_time.h"
#include "../FunEngine2D/core/include/render/window_manager.h"
#include "../FunEngine2D/core/include/networking/client.h"
#include "../FunEngine2D/core/include/networking/server.h"
#include "../FunEngine2D/core/include/tools/command.h"
#include "../FunEngine2D/core/include/tools/debugger.h"
#include "../FunEngine2D/core/include/_math.h"

#include "../common/include/space.h"

#include "include/chunk.h"
#include "include/canvas.h"

#include "include/slave.h"

int main () {
    fun::winmgr::init(fun::winmgr::window_data_t("Place Server"));
    auto* window = fun::winmgr::main_window;

    fun::server_t server;
    server.launch(8001);

    space::canvas_t canvas;
    
    while (window->render.isOpen()) {
        fun::time::recalculate();
        fun::winmgr::update();

        server.listen();
        auto& packet_storage = server.get_packets();

        if (!packet_storage.empty()) {
            fun::packet_storage_t::packet_t packet = packet_storage.read();

            space::slave::process(server, canvas, fun::command_t(packet.data), packet.sender);
        }

        {
            auto world = window->get_mouse_world_position();
            auto grid = space::world_to_grid(world);
            auto chunk = space::grid_to_chunk(grid);
            auto texel = space::grid_to_texel(grid);

            println(world << ' ' << grid << ' ' << chunk << ' ' << texel);
        }

        fun::debugger::display();

        window->display(sf::Color::Black);
    }

    server.terminate();

    fun::winmgr::close();

    return 0;
};