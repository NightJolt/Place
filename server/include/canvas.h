#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/vec2.h"
#include "../../FunEngine2D/core/include/_math.h"

#include "../../common/include/space.h"

#include "chunk.h"

namespace space {
    class canvas_t {
    public:

        chunk_t* get_chunk(fun::vec2_t <space::chunk_int_t>);

        fun::rgb_t get_color(fun::vec2_t <space::grid_int_t>);
        void set_color(fun::vec2_t <space::grid_int_t>, fun::rgb_t);

    private:

        fun::unordered_map_vec2_t <chunk_int_t, chunk_t*> chunks;
    };
}