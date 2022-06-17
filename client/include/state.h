#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/networking/client.h"

#include "canvas.h"

namespace space {
    enum tool_mode_t {
        brush,
        eyedrop,
        select
    };

    struct tool_t {
        tool_mode_t mode;
        fun::rgb_t color;
    };

    struct state_t {
        fun::client_t client;
        canvas_t canvas;
        tool_t tool;
    };
}