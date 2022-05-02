#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/color.h"

namespace space {
    typedef int16_t chunk_pos_t;
    typedef uint8_t chunk_size_t;
    typedef uint8_t texel_local_pos_t;
    typedef int32_t texel_global_pos_t;

    inline constexpr chunk_size_t chunk_size = 16;

    // fun::vec2i_t to_chunk_position(fun::vec2f_t);

    namespace chunk {
        std::string encode(chunk_pos_t, chunk_pos_t, fun::rgb_t*);
        std::vector <uint8_t> decode(std::string, chunk_pos_t*, chunk_pos_t*);
    }
}