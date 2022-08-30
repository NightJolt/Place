#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/tools/command.h"
#include "../../FunEngine2D/core/include/networking/client.h"

#include "state.h"

namespace space::slave {
    void set_clientname(state_t&, const std::string&);
    void send_texel(state_t&, grid_pos_t, fun::rgb_t);
    void send_message(state_t&, const std::string&);
    void request_chunk(state_t&, chunk_pos_t);

    void step(state_t&, float);
    
    void process(state_t&, const fun::network::packet_t&);
}