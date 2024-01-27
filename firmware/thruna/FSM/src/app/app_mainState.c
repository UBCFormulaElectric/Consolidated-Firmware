#include "app_mainState.h"
#include <stddef.h>
#include "app_utils.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_coolant.h"
#include "app_steering.h"
#include "app_wheels.h"
#include "app_heartbeatMonitor.h"

void mainStateRunOnTick100Hz(void)
{
    app_apps_broadcast();
    app_brake_broadcast();
    app_coolant_broadcast();
    app_steering_broadcast();
    app_wheels_broadcast();

    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_tick();
    app_heartbeatMonitor_broadcastFaults();

    bool missing_hb = app_heartbeatMonitor_checkFaults();
    if (missing_hb)
    {
        // Redundancy if FSM is missing heartbeats
        // Suppress accelerator pedal percentages (set to 0%)
        App_CanTx_FSM_PappsMappedPedalPercentage_Set(0);
        App_CanTx_FSM_SappsMappedPedalPercentage_Set(0);
    }
}

const State *app_mainState_get(void)
{
    static State drive_state = {
        .name              = "DRIVE STATE",
        .run_on_entry      = NULL,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = mainStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };
    return &drive_state;
}
