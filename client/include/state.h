#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/networking/client.h"

#include "canvas.h"

namespace space {
    struct brush_t {
        fun::rgb_t color;
    };

    struct state_t {
        fun::client_t client;

        canvas_t canvas;
        brush_t brush;
    };
}