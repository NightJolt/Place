#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/tools/command.h"
#include "../../FunEngine2D/core/include/networking/client.h"

#include "state.h"

namespace space::slave {
    void send_texel(state_t&, grid_pos_t, fun::rgb_t);
    void send_message(state_t&, const std::string&);
    void request_chunk(state_t&, chunk_pos_t);
    void request_all_chunks(state_t&);
    
    void send_message(state_t&, const std::string&);
    void process_command(state_t&, std::string&);

    void step(state_t&, float);
    
    void process(state_t&, const fun::network::packet_t&);
}