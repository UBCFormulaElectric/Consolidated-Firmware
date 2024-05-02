#include "app_switches.h"
#include "app_canTx.h"

static const Switches *switches = NULL;

void app_switches_init(const Switches *switches_in)
{
    switches = switches_in;
}

void app_switches_update(void)
{
    if (switches == NULL)
    {
        return;
    }

    const bool start_switch_on = io_switch_isClosed(switches->start_switch);
    app_canTx_CRIT_StartSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool regen_switch_on = io_switch_isClosed(switches->regen_switch);
    app_canTx_CRIT_RegenSwitch_set(regen_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool torquevec_switch_on = io_switch_isClosed(switches->torquevec_switch);
    app_canTx_CRIT_TorqueVecSwitch_set(torquevec_switch_on ? SWITCH_ON : SWITCH_OFF);
}
