#include "app/startSwitch.h"

#include "app_canRx.h"

bool app_startSwitch_hasRisingEdge(void)
{
    static SwitchState last_switch_state  = SWITCH_OFF;
    const SwitchState  start_switch_state = app_canRx_CRIT_StartSwitch_get();

    const bool has_rising_edge = (start_switch_state && !last_switch_state);
    last_switch_state          = start_switch_state;
    return has_rising_edge;
}