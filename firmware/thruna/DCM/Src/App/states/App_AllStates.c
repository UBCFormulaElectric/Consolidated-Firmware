#include "App_SharedMacros.h"
#include "states/App_FaultState.h"
#include "states/App_AllStates.h"
#include "App_EllipseImu.h"

#define IGNORE_HEARTBEAT_CYCLES 3U

static uint16_t num_cycles = 0;

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

    if (num_cycles <= IGNORE_HEARTBEAT_CYCLES)
    {
        num_cycles++;
    }

    const bool is_missing_hb = !App_SharedHeartbeatMonitor_Tick(hb_monitor) && num_cycles > IGNORE_HEARTBEAT_CYCLES;
    App_CanAlerts_SetFault(DCM_FAULT_MISSING_HEARTBEAT, is_missing_hb);

    if (App_HasInverterFault() || is_missing_hb)
    {
        status = false;
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    App_EllipseImu_HandleLogs();
    App_EllipseImu_Broadcast();

    return status;
}
