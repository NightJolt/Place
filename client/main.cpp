#include "../FunEngine2D/core/include/globals.h"
#include "../FunEngine2D/core/include/vec2.h"
#include "../FunEngine2D/core/include/_time.h"
#include "../FunEngine2D/core/include/render/window_manager.h"
#include "../FunEngine2D/core/include/tools/debugger.h"
#include "../FunEngine2D/core/include/networking/client.h"
#include "../FunEngine2D/core/include/networking/server.h"
#include "../FunEngine2D/core/include/input.h"

#include "../common/include/space.h"

#include "slave.h"
#include "canvas.h"
#include "interface.h"
#include "state.h"

int main () {
    fun::winmgr::init(fun::winmgr::window_data_t("Place Client"));
    auto* window = fun::winmgr::main_window;

    window->set_vsync(false);
    window->target_framerate(60);

    space::state_t state;
    state.tool.mode = space::tool_mode_t::brush;
    state.tool.color = fun::rgb_t::white;
    state.batch_max_texels = 256;
    state.batch_send_interval = 1.f;
    state.batch_cooldown = state.batch_send_interval;
    
    while (window->render.isOpen()) {
        fun::time::recalculate();
        fun::winmgr::update();
        fun::input::listen();

        window->world_view.move((fun::input::keyboard_2d() * fun::vec2f_t(1, -1) * window->zoom * 200.f * fun::time::delta_time()).to_sf());

        if (fun::input::pressed(sf::Keyboard::B)) state.tool.mode = space::tool_mode_t::brush;
        if (fun::input::pressed(sf::Keyboard::I)) state.tool.mode = space::tool_mode_t::eyedrop;
        if (fun::input::pressed(sf::Keyboard::E)) state.tool.mode = space::tool_mode_t::erase;

        if (fun::input::hold(sf::Mouse::Left)) {
            switch(state.tool.mode) {
                case space::tool_mode_t::brush: {
                    space::slave::send_texel(state, space::world_to_grid(window->get_mouse_world_position()), state.tool.color);

                    break;
                }
                
                case space::tool_mode_t::erase: {
                    space::slave::send_texel(state, space::world_to_grid(window->get_mouse_world_position()), fun::rgb_t::black);

                    break;
                }

                case space::tool_mode_t::eyedrop: {
                    state.tool.color = state.canvas.get_color(space::world_to_grid(window->get_mouse_world_position()));
                    state.tool.mode = space::tool_mode_t::brush;

                    break;
                }
            }
        }

        if (fun::input::pressed(sf::Keyboard::Space)) {
            space::slave::request_chunk(state, space::grid_to_chunk(space::world_to_grid(window->get_mouse_world_position())));
        }
        
        space::slave::step(state, fun::time::delta_time());

        space::interf::draw(state);
        
        // fun::debugger::display();

        window->draw_world(state.canvas, 0);

        window->display(sf::Color::Black);
    }

    state.client.disconnect();

    fun::winmgr::close();

    return 0;
};