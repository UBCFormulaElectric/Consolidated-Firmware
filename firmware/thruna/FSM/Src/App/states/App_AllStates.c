#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_DriveState.h"
#include "App_SharedConstants.h"

#define TORQUE_LIMIT_OFFSET_NM (5.0f)
#define MAX_TORQUE_PLAUSIBILITY_ERR_CNT (25) // 250 ms window

static bool App_SendAndReceiveHeartbeat(struct HeartbeatMonitor *hb_monitor)
{
    App_CanTx_FSM_Heartbeat_Set(true);

    if (App_CanRx_BMS_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Heartbeat_Update(false);
    }

    const bool missing_hb = !App_SharedHeartbeatMonitor_Tick(hb_monitor);
    return missing_hb;
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct Brake *   brake = App_FsmWorld_GetBrake(world);

    App_CanTx_FSM_BrakePressureSensorOCSC_Set(App_Brake_PressureElectricalFault(brake));
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *        world      = App_SharedStateMachine_GetWorld(state_machine);
    struct HeartbeatMonitor *hb_monitor = App_FsmWorld_GetHeartbeatMonitor(world);

    const bool missing_hb = App_SendAndReceiveHeartbeat(hb_monitor);
    App_CanAlerts_FSM_Fault_MissingHeartbeat_Set(missing_hb);

    if (missing_hb)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    App_AcceleratorPedals_Broadcast(world);
    App_Brake_Broadcast(world);
    App_Coolant_Broadcast(world);
    App_Steering_Broadcast(world);
    App_Wheels_Broadcast(world);
}
