#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/color.h"

namespace space {
    typedef int16_t chunk_pos_t;
    typedef int8_t chunk_size_t;
    typedef int8_t texel_local_pos_t;
    typedef int32_t texel_global_pos_t;

    static constexpr chunk_size_t chunk_size_x = 16;
    static constexpr chunk_size_t chunk_size_y = 16;

    namespace chunk {
        std::string encode(chunk_pos_t, chunk_pos_t, chunk_size_t, chunk_size_t, fun::rgb_t*);

        // template <texel_local_pos_t X, texel_local_pos_t Y>
        // std::vector <uint8_t>  decode(std::string data, int* x, int* y) {
        //     static constexpr size_t mem_size = sizeof chunk_pos_t * 2 + X * Y * sizeof fun::rgb_t + 1;

        //     void* ptr = malloc(mem_size);
        //     uint32_t pos = 0;

        //     *(chunk_pos_t)ptr = x;
        //     *(chunk_pos_t)(ptr + sizeof chunk_pos_t) = y;

        //     pos += sizeof chunk_pos_t * 2;

        //     for (texel_local_pos_t x = 0; x < size_x; x++) {
        //         for (texel_local_pos_t y = 0; y < size_y; y++) {
        //             fun::rgb_t* color = data[x][y];

        //             *(uint8_t)(ptr + pos).r = color.r;
        //             pos += sizeof uint8_t;

        //             *(uint8_t)(ptr + pos).g = color.g;
        //             pos += sizeof uint8_t;

        //             *(uint8_t)(ptr + pos).b = color.b;
        //             pos += sizeof uint8_t;
        //         }
        //     }

        //     *(uint8_t)(ptr + pos) = '\n';
        // }
    }
}