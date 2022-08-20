#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/tools/command.h"
#include "../../FunEngine2D/core/include/networking/client.h"

#include "state.h"

namespace space::slave {
    void set_clientname(state_t&, const std::string&);
    void send_texel(state_t&, fun::vec2_t <grid_pos_t>, fun::rgb_t);
    void send_message(state_t&, const std::string&);
    
    void process_command(state_t&, const std::string&);
    void process_command(state_t&, const fun::command_t&);
}