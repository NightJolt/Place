#include "../FunEngine2D/core/include/globals.h"
#include "../FunEngine2D/core/include/vec2.h"
#include "../FunEngine2D/core/include/_time.h"
#include "../FunEngine2D/core/include/render/window_manager.h"
#include "../FunEngine2D/core/include/networking/client.h"
#include "../FunEngine2D/core/include/networking/server.h"
#include "../FunEngine2D/core/include/input.h"

#include "../common/include/space.h"

#include "include/slave.h"
#include "include/canvas.h"

#include "include/interface.h"

int main () {
    fun::winmgr::init(fun::winmgr::window_data_t("Place Client"));
    auto* window = fun::winmgr::main_window;

    window->set_vsync(false);
    // ! window->target_framerate(60);

    // {
    //     println((fun::vec2f_t)space::chunk_to_grid({ 3, -2 }));

    //     exit(0);
    // }

    fun::client_t client;
    assert(client.connect("localhost", 8001));

    space::canvas_t canvas;
    
    while (window->render.isOpen()) {
        fun::time::recalculate();
        fun::winmgr::update();
        fun::input::listen();

        window->world_view.move((fun::input::keyboard_2d() * fun::vec2f_t(1, -1) * window->zoom * 200.f * fun::time::delta_time()).to_sf());

        if (fun::input::hold(sf::Mouse::Left)) space::slave::send_texel(client, canvas, space::world_to_grid(window->get_mouse_world_position()), space::interf::get_selected_color());

        client.receive();
        
        auto& packet_storage = client.get_packets();

        if (!packet_storage.empty()) {
            space::slave::process(client, canvas, fun::command_t(packet_storage.read().data));
        }

        space::interf::draw();

        window->draw_world(canvas, 0);

        window->display(sf::Color::Black);
    }

    client.disconnect();

    fun::winmgr::close();

    return 0;
};