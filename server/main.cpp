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
#include "../FunEngine2D/core/include/pointer.h"

#include "../common/include/space.h"

#include "include/chunk.h"
#include "include/canvas.h"

#include "include/slave.h"

int main () {
    fun::winmgr::init(fun::winmgr::window_data_t("Place Server"));
    auto* window = fun::winmgr::main_window;

    window->set_vsync(false);
    window->target_framerate(60);

    fun::server_t server;
    server.launch(8001);

    space::canvas_t canvas;

    const uint32_t max_requests_per_frame = 100;
    uint32_t current_requests_left;
    
    while (window->render.isOpen()) {
        fun::time::recalculate();
        fun::winmgr::update();

        server.listen();
        auto& packet_storage = server.get_packets();

        current_requests_left = max_requests_per_frame;

        while (current_requests_left-- && !packet_storage.empty()) {
            fun::packet_storage_t::packet_t packet = packet_storage.read();

            space::slave::process(server, canvas, fun::command_t(packet.data), packet.sender);
        }
        
        fun::debugger::display();

        window->display(sf::Color::Black);
    }

    server.terminate();

    fun::winmgr::close();

    return 0;
};