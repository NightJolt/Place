#pragma once

#include "../../FunEngine2D/core/include/globals.h"
#include "../../FunEngine2D/core/include/tools/command.h"
#include "../../FunEngine2D/core/include/networking/server.h"

#include "state.h"
// todo
// process to process_command
// server and canvas to state

namespace space::slave {
    void run(uint32_t, state_t*);
}