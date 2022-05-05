#pragma once

#include "../../FunEngine2D/core/include/vec2.h"
#include "../../FunEngine2D/core/include/_math.h"

#include "../../common/include/space.h"

#include "chunk.h"

namespace space {
    class canvas_t {
    public:

        chunk_t* get_chunk(space::chunk_pos_t, space::chunk_pos_t);

        fun::rgb_t get_color(space::grid_pos_t, space::grid_pos_t);
        void set_color(space::grid_pos_t, space::grid_pos_t, fun::rgb_t);

    private:

        std::unordered_map <
            fun::vec2_t <space::chunk_pos_t>,
            chunk_t*,
            decltype([](const fun::vec2_t <space::chunk_pos_t>& v) -> size_t const { return fun::hash(v); }),
            decltype([](const fun::vec2_t <space::chunk_pos_t>& a, const fun::vec2_t <space::chunk_pos_t>& b) -> bool const { return a == b; })
        > data;
    };
}