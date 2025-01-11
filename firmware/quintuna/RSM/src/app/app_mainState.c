#include <stddef.h>

#include "app_canTx.h"
#include "app_mainState.h"
#include "app_canRx.h"
//#include "app_coolant.h"
//#include "app_loadCell.h"
//#include "app_suspension.h"
//#include "app_heartbeatMonitors.h"

#include "io_fans.h"
#include "io_leds.h"
#include "io_led.h"

void mainStateRunOnTick100Hz(void)
{
    /*
    app_coolant_broadcast();
    app_loadcell_broadcast();
    app_suspension_broadcast();
    app_heartbeatMonitor_checkIn(&hb_monitor);
    app_heartbeatMonitor_broadcastFaults(&hb_monitor);
    */

    const bool brake_light_on = app_canRx_FSM_BrakeActuated_get();
    io_led_enable(brake_light, brake_light_on);
    app_canTx_RSM_BrakeLight_set(brake_light_on);    

    const bool hv_on = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    io_fan_set(rad_fan, hv_on);
    io_fan_set(acc_fan, hv_on);
    app_canTx_RSM_AccumulatorFan_set(hv_on);
    app_canTx_RSM_RadiatorFan_set(hv_on);
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
