#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/color.h"
#include "../../FunEngine2D/core/include/vec2.h"
#include "../../FunEngine2D/core/include/_math.h"

namespace space {
    typedef int32_t grid_int_t;
    typedef int32_t chunk_int_t;
    typedef uint8_t texel_int_t;

    typedef fun::vec2_t <grid_int_t> grid_pos_t;
    typedef fun::vec2_t <chunk_int_t> chunk_pos_t;
    typedef fun::vec2_t <texel_int_t> texel_pos_t;

    inline constexpr texel_int_t chunk_size = 16;

    texel_pos_t array_to_texel(int32_t);
    int32_t texel_to_array(texel_pos_t);

    grid_pos_t world_to_grid(fun::vec2f_t);
    chunk_pos_t grid_to_chunk(grid_pos_t);
    grid_pos_t chunk_to_grid(chunk_pos_t);
    texel_pos_t grid_to_texel(grid_pos_t);

    namespace chunk {
        std::string encode(chunk_int_t, chunk_int_t, fun::rgb_t*);
        std::vector <fun::rgb_t> decode(std::string, chunk_int_t*, chunk_int_t*);
    }

#pragma pack(push, 1)
    enum server_cmd_t {
        set_pixel,
        get_pixel,

        send_chunk,
        get_chunk,

        send_batch,

        count
    };
#pragma pack(pop)
} 