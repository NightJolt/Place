#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/_math.h"

namespace space {
    struct statistics_t {
        statistics_t();

        uint32_t texels_placed;
    };

    struct client_data_t {
        client_data_t();

        fun::str_t name;
        uint32_t texels_placed;
    };
}