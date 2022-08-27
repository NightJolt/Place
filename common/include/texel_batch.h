#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/vec2.h"

#include "../../common/include/space.h"

namespace space {
    class texel_batch_t {
    private:
        struct texel_t {
            texel_pos_t pos;
            fun::rgb_t color;
        };

    public:
        texel_batch_t();

        void add_texel(grid_pos_t, fun::rgb_t);
        void add_texel(chunk_pos_t, texel_pos_t, fun::rgb_t);

        fun::str_t to_str();
        void from_str(const fun::str_t&);

        uint32_t get_total_texels();

        fun::unordered_map_vec2_t <chunk_int_t, std::vector <texel_t>>& get_data();

        void clear();

    private:
        fun::unordered_map_vec2_t <chunk_int_t, std::vector <texel_t>> m_data;

        uint32_t m_total_texels;
    };
}