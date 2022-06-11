#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/tools/command.h"
#include "../../FunEngine2D/core/include/networking/client.h"

#include "canvas.h"

namespace space::slave {
    void send_texel(fun::client_t&, canvas_t&, fun::vec2_t <grid_pos_t>, fun::rgb_t);
    
    void process(fun::client_t&, canvas_t&, const fun::command_t&);
}