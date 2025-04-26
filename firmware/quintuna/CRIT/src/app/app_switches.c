#include "app_switches.h"
#include "io_switches.h"
#include "app_canTx.h"

/**
 * @brief Broadcasts switch state.
 */
void app_switches_broadcast(void)
{
    const bool push_drive_on = io_switch_isClosed(&push_drive_switch);
    app_canTx_CRIT_StartSwitch_set(push_drive_on ? SWITCH_ON : SWITCH_OFF);

    const bool regen_switch_on = io_switch_isClosed(&regen_switch);
    app_canTx_CRIT_RegenSwitch_set(regen_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool torquevec_switch_on = io_switch_isClosed(&torque_vectoring_switch);
    app_canTx_CRIT_TorqueVecSwitch_set(torquevec_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool launch_control_switch_on = io_switch_isClosed(&launch_control_switch);
    app_canTx_CRIT_LaunchControlSwitch_set(launch_control_switch_on ? SWITCH_ON : SWITCH_OFF);

    const bool telem_switch_on = io_switch_isClosed(&telem_switch);
    app_canTx_CRIT_TelemSwitch_set(telem_switch_on ? SWITCH_ON : SWITCH_OFF);
}
