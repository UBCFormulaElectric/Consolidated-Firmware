#include <string.h>
#include "app_mainState.h"
#include <stdlib.h>
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_globals.h"
#include "io_led.h"
#include "io_switch.h"

static void mainStateRunOnTick100Hz(void)
{
    // TODO: reading faults from the latches: need to wait for BMS
    // const bool imd_fault_latched =
    // io_led_enable(globals->config->imd_led, imd_fault_latched);

    // const bool bspd_fault_latched =
    // io_led_enable(globals->config->bspd_led, bspd_fault_latched);

    // const bool ams_fault_latched =
    // io_led_enable(globals->config->ams_led, ams_fault_latched);

    const bool start_switch_on = io_switch_isClosed(globals->config->start_switch);
    app_canTx_CRIT_StartSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    io_led_enable(globals->config->start_led, start_switch_on);

    const bool regen_switch_on = io_switch_isClosed(globals->config->regen_switch);
    app_canTx_CRIT_RegenSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    io_led_enable(globals->config->regen_led, regen_switch_on);

    const bool torquevec_switch_on = io_switch_isClosed(globals->config->torquevec_switch);
    app_canTx_CRIT_TorqueVecSwitch_set(start_switch_on ? SWITCH_ON : SWITCH_OFF);
    io_led_enable(globals->config->torquevec_led, torquevec_switch_on);
}
const State *app_mainState_get(void)
{
    static const State main_state = {
        .name              = "MAIN",
        .run_on_entry      = NULL,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = mainStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &main_state;
}