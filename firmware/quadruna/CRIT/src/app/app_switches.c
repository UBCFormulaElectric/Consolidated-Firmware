#include "app_switches.h"
#include "io_switches.h"
#include "app_canTx.h"

void app_switches_broadcast(void)
{
    const bool start_switch_on = io_switches_start_get();
    app_canTx_CRIT_StartSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool regen_switch_on = io_switches_regen_get();
    app_canTx_CRIT_RegenSwitch_set(regen_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool torquevec_switch_on = io_switches_torquevec_get();
    app_canTx_CRIT_TorqueVecSwitch_set(torquevec_switch_on ? SWITCH_ON : SWITCH_OFF);
}
