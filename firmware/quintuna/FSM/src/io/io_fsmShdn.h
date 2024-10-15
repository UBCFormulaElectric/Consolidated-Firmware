#pragma once

extern "C"
{
#include "app_canTx.h"
}
#include "io_shdnLoopNode.h"
#include "hw_gpios.h"

namespace io::shdn
{
const node cockpit_estop_node{ hw::gpio::fsm_shdn, app_canTx_FSM_FSMShdnNodeStatus_set };
} // namespace io::shdn
