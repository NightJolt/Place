#pragma once

#include "../../FunEngine2D/core/include/globals.h"

namespace space {
    struct statistics_t {
        uint32_t texels_placed;
    };

    struct client_data_t {
        std::string name;
        uint32_t texels_placed;
    };
}