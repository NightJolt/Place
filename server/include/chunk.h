#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/vec2.h"

#include "../../common/include/space.h"

namespace space {
    class chunk_t {
    public:

        chunk_t();

        fun::rgb_t get_color(fun::vec2_t <space::texel_int_t>);
        void set_color(fun::vec2_t <space::texel_int_t>, fun::rgb_t);

        fun::rgb_t* get_data();

    private:

        fun::rgb_t data[chunk_size][chunk_size];
    };
}