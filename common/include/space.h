#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/color.h"
#include "../../FunEngine2D/core/include/vec2.h"
#include "../../FunEngine2D/core/include/_math.h"

namespace space {
    typedef int32_t grid_pos_t;
    typedef int16_t chunk_pos_t;
    typedef uint8_t texel_pos_t;

    inline constexpr texel_pos_t chunk_size = 16;

    fun::vec2_t <texel_pos_t> array_to_texel(uint16_t);
    uint16_t texel_to_array(fun::vec2_t <texel_pos_t>);

    fun::vec2_t <grid_pos_t> world_to_grid(fun::vec2f_t);
    fun::vec2_t <chunk_pos_t> grid_to_chunk(fun::vec2_t <grid_pos_t>);
    fun::vec2_t <grid_pos_t> chunk_to_grid(fun::vec2_t <chunk_pos_t>);
    fun::vec2_t <texel_pos_t> grid_to_texel(fun::vec2_t <grid_pos_t>);

    namespace chunk {
        std::string encode(chunk_pos_t, chunk_pos_t, fun::rgb_t*);
        std::vector <fun::rgb_t> decode(std::string, chunk_pos_t*, chunk_pos_t*);
    }
} 