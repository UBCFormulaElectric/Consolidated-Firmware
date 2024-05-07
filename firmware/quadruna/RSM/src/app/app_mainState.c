#include <stddef.h>
#include "io_fan.h"
#include "io_brake_light.h"
#include "app_canTx.h"
#include "app_mainState.h"
#include "app_canRx.h"
#include "app_coolant.h"
#include "app_loadCell.h"
#include "app_suspension.h"
#include "app_heartbeatMonitor.h"

void mainStateRunOnTick100Hz(void)
{
    app_coolant_broadcast();
    app_loadcell_broadcast();
    app_suspension_broadcast();
    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_broadcastFaults();

    io_brake_light_set(app_canRx_FSM_BrakeActuated_get());
    const bool hv_on = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    io_acc_fan_set(hv_on);
    io_rad_fan_set(hv_on);
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
