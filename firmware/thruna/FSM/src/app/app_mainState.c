#include "app_mainState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "app_utils.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "app_globals.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_coolant.h"
#include "app_steering.h"
#include "app_wheels.h"

#define TORQUE_LIMIT_OFFSET_NM (5.0f)
#define MAX_TORQUE_PLAUSIBILITY_ERR_CNT (25) // 250 ms window

static bool sendAndReceiveHeartbeat(void)
{
    App_SharedHeartbeatMonitor_CheckIn(globals->heartbeat_monitor);

    App_SharedHeartbeatMonitor_Tick(globals->heartbeat_monitor);
    App_SharedHeartbeatMonitor_BroadcastFaults(globals->heartbeat_monitor);

    bool missing_hb = false;
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        missing_hb |= !globals->heartbeat_monitor->status[board];
    }

    return missing_hb;
}

void mainStateRunOnTick100Hz(void)
{
    // Check for torque plausibility
    float left_torque_req  = (float)App_CanRx_DCM_LeftInverterTorqueCommand_Get();
    float right_torque_req = (float)App_CanRx_DCM_RightInverterTorqueCommand_Get();

    static uint8_t error_count = 0;

    App_CanAlerts_FSM_Fault_TorquePlausabilityFailed_Set(error_count >= MAX_TORQUE_PLAUSIBILITY_ERR_CNT);

    // Broadcast a new FSM torque limit based on pedal percentage

    app_apps_broadcast();
    app_brake_broadcast();
    app_coolant_broadcast();
    app_steering_broadcast();
    app_wheels_broadcast();

    const bool missing_hb = sendAndReceiveHeartbeat();

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
