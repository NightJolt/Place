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
        std::vector <uint8_t> decode(std::string, chunk_size_t, chunk_size_t, chunk_pos_t*, chunk_pos_t*);
    }
}