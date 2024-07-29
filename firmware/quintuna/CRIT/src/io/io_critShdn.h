#pragma once

extern "C"
{
#include "app_canTx.h"
}
#include "io_shdnLoopNode.h"
#include "hw_gpios.h"

namespace io::shdn
{
const node cockpit_estop_node{ hw::gpio::cockpit_estop_sen_pin, app_canTx_CRIT_CockpitEStopOKStatus_set };
const node inertia_sen_node{ hw::gpio::inertia_sen_pin, app_canTx_CRIT_InertiaSenOKStatus_set };
} // namespace io::shdn
