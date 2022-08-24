#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/networking/server.h"

#include "canvas.h"
#include "data.h"

namespace space {
    struct state_t {
        fun::network::server_t server;
        canvas_t canvas;
        statistics_t statistics;
        std::unordered_map <sf::TcpSocket*, client_data_t> client_datas;
    };
}