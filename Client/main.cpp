#include "../FunEngine2D/core/include/globals.h"
#include "../FunEngine2D/core/include/vec2.h"
#include "../FunEngine2D/core/include/_time.h"
#include "../FunEngine2D/core/include/render/window_manager.h"
#include "../FunEngine2D/core/include/networking/client.h"
#include "../FunEngine2D/core/include/networking/server.h"

int main () {
    fun::winmgr::init(fun::winmgr::window_data_t("Place Client"));
    auto* window = fun::winmgr::main_window;

    fun::client_t client;
    if (!client.connect("localhost", 8001)) exit(69);
    
    while (window->render.isOpen()) {
        fun::time::recalculate();
        fun::winmgr::update();

        window->display(sf::Color::Black);
    }

    client.disconnect();

    fun::winmgr::close();

    return 0;
};