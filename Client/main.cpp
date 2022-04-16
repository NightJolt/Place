#include "../FunEngine2D/core/include/globals.h"

int main() {
    println("Working");

    return 0;
}

// #include "FunEngine2D/core/include/globals.h"

// #include "../FunEngine2D/"

// #include "FunEngine2D/core/include/vec2.h"
// #include "FunEngine2D/core/include/transform.h"
// #include "FunEngine2D/core/include/render/window_manager.h"
// #include "FunEngine2D/core/include/resources.h"
// #include "FunEngine2D/core/include/_time.h"
// #include "FunEngine2D/core/include/input.h"
// #include "FunEngine2D/core/include/tools/debugger.h"
// #include "FunEngine2D/core/include/render/shapes/particler.h"

// #include "FunEngine2D/core/include/networking/client.h"
// #include "FunEngine2D/core/include/networking/server.h"
// #include "FunEngine2D/core/include/ecs/ecs.h"
// #include "FunEngine2D/core/include/physics/physics.h"
// #include "FunEngine2D/core/include/interact/interaction.h"
// #include "FunEngine2D/core/include/interact/interactable.h"

// int main () {
//     fun::winmgr::init(fun::winmgr::window_data_t("Spaceships"));
//     auto* window = fun::winmgr::main_window;
    
//     while (window->render.isOpen()) {
//         fun::time::recalculate();
//         fun::input::listen();
//         fun::interaction::update();
//         fun::winmgr::update();

//         window->world_view.move((fun::input::keyboard_2d() * fun::vec2f_t(1, -1) * window->zoom * 200.f * fun::time::delta_time()).to_sf());

        

//         fun::debugger::display();

//         window->display(sf::Color::Black);
//     }

//     fun::winmgr::close();

//     return 0;
// };