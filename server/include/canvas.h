#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/vec2.h"
#include "../../FunEngine2D/core/include/_math.h"

#include "../../common/include/space.h"

#include "chunk.h"

namespace space {
    class canvas_t {
    private:
        typedef fun::unordered_map_vec2_t <chunk_int_t, chunk_t*> chunk_map_t;

    public:
        void init_chunk(chunk_pos_t);
        bool has_chunk(chunk_pos_t);

        chunk_t* get_chunk(chunk_pos_t);

        fun::rgb_t get_color(grid_pos_t);
        fun::rgb_t get_color(chunk_pos_t, texel_pos_t);

        void set_color(grid_pos_t, fun::rgb_t);
        void set_color(chunk_pos_t, texel_pos_t, fun::rgb_t);

        chunk_map_t& get_chunks();

        std::shared_mutex key;

    private:
        chunk_map_t m_chunks;
    };
}