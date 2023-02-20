#include "App_SharedMacros.h"
#include "states/App_FaultState.h"
#include "states/App_AllStates.h"

static void App_SendAndReceiveHeartbeat(struct HeartbeatMonitor *hb_monitor)
{
    App_CanTx_DCM_Vitals_Heartbeat_Set(true);
    if (App_CanRx_BMS_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Vitals_Heartbeat_Update(false);
    }

    const bool is_missing_hb = !App_SharedHeartbeatMonitor_Tick(hb_monitor);
    App_CanTx_DCM_Errors_MissingHeartbeat_Set(is_missing_hb);
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
    App_BrakeLight_SetLightStatus(brake_light, App_CanRx_FSM_Test_IsBrakeActuated_Get());

    if (App_HasInverterFault())
    {
        status = false;
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    return status;
}
