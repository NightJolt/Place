#pragma once

#include "../../FunEngine2D/core/include/vec2.h"
#include "../../FunEngine2D/core/include/_math.h"

#include "../../common/include/space.h"

#include "chunk.h"

namespace space {
    class canvas_t {
    public:

        chunk_t* get_chunk(fun::vec2_t <space::chunk_pos_t>);

        fun::rgb_t get_color(fun::vec2_t <space::texel_global_pos_t>);
        void set_color(fun::vec2_t <space::texel_global_pos_t>, fun::rgb_t);

    private:

        std::unordered_map <
            fun::vec2i_t,
            chunk_t*,
            decltype([](const fun::vec2i_t& v) -> size_t const { return std::hash <fun::vec2i_t> () (v); }),
            decltype([](const fun::vec2i_t& a, const fun::vec2i_t& b) -> bool const { return a == b; })
        > data;
    };
}