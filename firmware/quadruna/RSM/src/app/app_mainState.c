#include <stddef.h>
#include "io_led.h"
#include "app_mainState.h"
#include "app_canRx.h"
#include "app_coolant.h"
#include "app_loadCell.h"
#include "app_suspension.h"
#include "app_globals.h"

#include "app_heartbeatMonitor.h"

void mainStateRunOnTick100Hz(void)
{
    app_coolant_broadcast();
    app_loadcell_broadcast();
    app_suspension_broadcast();
    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_tick();
    app_heartbeatMonitor_broadcastFaults();

    const bool brake_actuated = app_canRx_FSM_BrakeActuated_get();
    io_led_enable(globals->config->brake_light, brake_actuated);

    const bool hv_on = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    hw_gpio_writePin(globals->config->acc_fan, hv_on);
    hw_gpio_writePin(globals->config->rad_fan, hv_on);

    bool missing_hb = app_heartbeatMonitor_checkFaults();
}

const State *app_mainState_get(void)
{
    static State main_state = {
        .name              = "MAIN STATE",
        .run_on_entry      = NULL,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = mainStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };
    return &main_state;
}
