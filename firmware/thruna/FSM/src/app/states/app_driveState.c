#include "states/app_driveState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "App_SharedMacros.h"
#include "App_SharedConstants.h"
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
    App_CanTx_FSM_Heartbeat_Set(true);

    if (App_CanRx_BMS_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(globals->heartbeat_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Heartbeat_Update(false);
    }

    const bool missing_hb = !App_SharedHeartbeatMonitor_Tick(globals->heartbeat_monitor);
    return missing_hb;
}

void driveStateRunOnEntry(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
    App_CanTx_FSM_State_Set(FSM_STATE_DRIVE);
}

void driveStateRunOnTick1Hz(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

void driveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    // Check for torque plausibility
    float left_torque_req  = (float)App_CanRx_DCM_LeftInverterTorqueCommand_Get();
    float right_torque_req = (float)App_CanRx_DCM_RightInverterTorqueCommand_Get();
    float fsm_torque_limit = App_CanTx_FSM_TorqueLimit_Get();

    static uint8_t error_count = 0;
    if (left_torque_req > fsm_torque_limit || right_torque_req > fsm_torque_limit)
    {
        error_count++;
    }
    else
    {
        error_count = 0;
    }

    App_CanAlerts_FSM_Fault_TorquePlausabilityFailed_Set(error_count >= MAX_TORQUE_PLAUSIBILITY_ERR_CNT);

    // Broadcast a new FSM torque limit based on pedal percentage
    fsm_torque_limit =
        0.01f * App_CanTx_FSM_PappsMappedPedalPercentage_Get() * MAX_TORQUE_REQUEST_NM + TORQUE_LIMIT_OFFSET_NM;
    App_CanTx_FSM_TorqueLimit_Set(fsm_torque_limit);

    app_apps_broadcast();
    app_brake_broadcast();
    app_coolant_broadcast();
    app_steering_broadcast();
    app_wheels_broadcast();

    const bool missing_hb = sendAndReceiveHeartbeat();
    App_CanAlerts_FSM_Fault_MissingHeartbeat_Set(missing_hb);
    if (missing_hb)
    {
        // Redundancy if FSM is missing heartbeats
        // Suppress accelerator pedal percentages (set to 0%)
        App_CanTx_FSM_PappsMappedPedalPercentage_Set(0);
        App_CanTx_FSM_SappsMappedPedalPercentage_Set(0);
    }
}

void driveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *app_driveState_get(void)
{
    static struct State drive_state = {
        .name              = "DRIVE STATE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };
    return &drive_state;
}
