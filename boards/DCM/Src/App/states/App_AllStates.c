#include "App_SharedMacros.h"
#include "states/App_FaultState.h"
#include "states/App_AllStates.h"
#include "App_SbgEllipseN.h"

static void App_SendAndReceiveHeartbeat(struct HeartbeatMonitor *hb_monitor)
{
    App_CanTx_DCM_Vitals_Heartbeat_Set(true);

    if (App_CanRx_BMS_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Vitals_Heartbeat_Update(false);
    }
    if (App_CanRx_FSM_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, FSM_HEARTBEAT_ONE_HOT);
        App_CanRx_FSM_Vitals_Heartbeat_Update(false);
    }
    if (App_CanRx_DIM_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, DIM_HEARTBEAT_ONE_HOT);
        App_CanRx_DIM_Vitals_Heartbeat_Update(false);
    }
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    bool                     status      = true;
    struct DcmWorld *        world       = App_SharedStateMachine_GetWorld(state_machine);
    struct BrakeLight *      brake_light = App_DcmWorld_GetBrakeLight(world);
    struct HeartbeatMonitor *hb_monitor  = App_DcmWorld_GetHeartbeatMonitor(world);

    App_SendAndReceiveHeartbeat(hb_monitor);

    const bool brake_actuated = App_CanRx_FSM_Brake_IsActuated_Get();
    App_BrakeLight_SetLightStatus(brake_light, brake_actuated);

    const bool is_missing_hb = !App_SharedHeartbeatMonitor_Tick(hb_monitor);
    App_CanTx_DCM_Warnings_MissingHeartbeat_Set(is_missing_hb);

    if (App_HasInverterFault() || is_missing_hb)
    {
        status = false;
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    App_SbgEllipseN_HandleLogs();
    App_SbgEllipseN_Broadcast();

    return status;
}
