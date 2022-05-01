#pragma once

#include "../../FunEngine2D/core/include/vec2.h"
#include "../../common/include/chunk.h"

namespace space {
    class chunk_t {
    public:

        chunk_t();

        fun::rgb_t get_color(uint8_t, uint8_t);
        void set_color(uint8_t, uint8_t, fun::rgb_t);

        fun::rgb_t* get_data();

    private:

        fun::rgb_t data[chunk_size_x][chunk_size_y];
    };
}