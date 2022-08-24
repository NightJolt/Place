#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/networking/client.h"

#include "canvas.h"
#include "texel_batch.h"

namespace space {
    enum tool_mode_t {
        brush,
        eyedrop,
        select
    };

    struct tool_t {
        tool_mode_t mode;
        fun::rgb_t color;
        // bool fill_line_gaps;
        // fun::vec2f_t last_position;
    };

    struct state_t {
        fun::network::client_t client;

        canvas_t canvas;
        tool_t tool;

        texel_batch batch;
        float batch_send_interval;
        float batch_cooldown;
        float batch_max_texels;

        std::vector <std::string> messages;
    };
}