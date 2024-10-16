#pragma once

extern "C"
{
#include "app_canTx.h"
}
#include "io_shdnLoopNode.h"
#include "hw_gpios.h"

namespace io::shdn
{
const node tsms_fault_node{ hw::gpio::tsms_shdn_sns, app_canTx_VC_TSMSOKStatus_set };
const node le_stop_node{ hw::gpio::l_shdn_sns, app_canTx_CRIT_LEStopOKStatus_set };
const node re_stop_node{ hw::gpio::r_shdn_sns, app_canTx_CRIT_REStopOKStatus_set };
const node splitter_box_interlock_node{ hw::gpio::sb_ilck_shdn_sns, app_canTx_CRIT_SplitterBoxInterlockOKStatus_set };
} // namespace io::shdn
