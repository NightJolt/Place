#pragma once

#include "../../FunEngine2D/core/include/tools/command.h"
#include "../../FunEngine2D/core/include/networking/server.h"
#include "canvas.h"

namespace space::slave {
    void process(fun::server_t&, canvas_t&, const fun::command_t&, sf::TcpSocket*);

    uint32_t get_texels_set_count();
}