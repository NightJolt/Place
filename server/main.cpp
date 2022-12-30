#include "../FunEngine2D/core/include/globals.h"
#include "../FunEngine2D/core/include/vec2.h"
#include "../FunEngine2D/core/include/color.h"
#include "../FunEngine2D/core/include/_time.h"
#include "../FunEngine2D/core/include/render/window/window_manager.h"
#include "../FunEngine2D/core/include/render/window/window_data.h"
#include "../FunEngine2D/core/include/render/window/window.h"
#include "../FunEngine2D/core/include/networking/client.h"
#include "../FunEngine2D/core/include/networking/server.h"
#include "../FunEngine2D/core/include/tools/command.h"
#include "../FunEngine2D/core/include/tools/debugger.h"
#include "../FunEngine2D/core/include/_math.h"

#include "../common/include/space.h"

#include "include/chunk.h"
#include "include/canvas.h"
#include "include/state.h"
#include "include/slave.h"

int main () {
    fun::render::winmgr::init(fun::render::window_data_t("Place Server"));
    auto& window = fun::render::winmgr::get_main_window();

    window.set_vsync(false);
    window.target_framerate(60);
    
    space::state_t state;
    state.server.launch(8001);

    space::slave::run(1, &state);
    
    while (window.is_open()) {
        fun::time::recalculate();
        fun::render::winmgr::update();

        state.server.listen();
        
        fun::debugger::display();

        window.display(sf::Color::Black);
    }

    state.server.terminate();

    // make sure all canvas related work is finished
    state.canvas.key.lock();

    fun::render::winmgr::close();

    return 0;
};