#include "app_switches.h"
#include "io_switches.h"
extern "C"
{
#include "app_canTx.h"
}

namespace app::switches
{
void broadcast()
{
    app_canTx_CRIT_StartSwitch_set(io::switches::start_switch.isClosed() ? SWITCH_ON : SWITCH_OFF);
    app_canTx_CRIT_RegenSwitch_set(io::switches::regen_switch.isClosed() ? SWITCH_ON : SWITCH_OFF);
    app_canTx_CRIT_TorqueVecSwitch_set(io::switches::torquevec_switch.isClosed() ? SWITCH_ON : SWITCH_OFF);
}
} // namespace app::switches