#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/networking/client.h"

#include "canvas.h"
#include "../../common/include/texel_batch.h"

namespace space {
    enum tool_mode_t {
        brush,
        erase,
        eyedrop,
        select
    };

    struct tool_t {
        tool_mode_t mode;
        fun::rgb_t color;

        bool fill_line_gaps;
        fun::vec2f_t last_frame_mouse_position;
        bool last_frame_mouse_active;
        fun::vec2i_t last_grid_position;
    };

    struct state_t {
        fun::network::client_t client;

        canvas_t canvas;
        tool_t tool;

        texel_batch_t batch;
        float batch_send_interval;
        float batch_cooldown;
        float batch_max_texels;

        std::vector <std::string> messages;
    };
}