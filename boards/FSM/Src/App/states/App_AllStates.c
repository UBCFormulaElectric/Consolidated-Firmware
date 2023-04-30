#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_DriveState.h"
#include "App_SharedConstants.h"

#define TORQUE_LIMIT_OFFSET_NM (5.0f)
#define MAX_TORQUE_PLAUSIBILITY_ERR_CNT (25) // 250 ms window

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct Brake *   brake = App_FsmWorld_GetBrake(world);

    App_CanTx_FSM_Brake_PressureSensorOpenShortCircuit_Set(App_Brake_PressureElectricalFault(brake));
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *        world       = App_SharedStateMachine_GetWorld(state_machine);
    struct HeartbeatMonitor *hb_monitor  = App_FsmWorld_GetHeartbeatMonitor(world);
    static uint8_t           error_count = 0;

    // Check for torque plausibility
    // TODO: JSONCAN
    // float left_torque_req = (float)App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
    //     App_CanRx_DCM_INVL_COMMAND_MESSAGE_GetSignal_TORQUE_COMMAND_INVL(can_rx));

    // float right_torque_req = (float)App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
    //     App_CanRx_DCM_INVR_COMMAND_MESSAGE_GetSignal_TORQUE_COMMAND_INVR(can_rx));

    float left_torque_req  = 0;
    float right_torque_req = 0;

    float fsm_torque_limit = App_CanTx_FSM_Apps_TorqueLimit_Get();

    if (left_torque_req > fsm_torque_limit || right_torque_req > fsm_torque_limit)
        error_count++;
    else
        error_count = 0;

    if (error_count == MAX_TORQUE_PLAUSIBILITY_ERR_CNT)
    {
        App_CanTx_FSM_Apps_TorquePlausabilityFailed_Set(true);
    }

    // Broadcast a new FSM torque limit based on pedal percentage
    fsm_torque_limit =
        0.01f * App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get() * MAX_TORQUE_REQUEST_NM + TORQUE_LIMIT_OFFSET_NM;
    App_CanTx_FSM_Apps_TorqueLimit_Set(fsm_torque_limit);

    // Debug msgs, remove after testing
    // App_CanTx_FSM_APPs_PappsMappedPedalPercentage_Set((uint16_t)
    // App_AcceleratorPedals_GetPrimaryPedalPercentage(accelerator_pedals));
    // App_CanTx_FSM_APPs_SappsMappedPedalPercentage_Set((uint16_t)App_AcceleratorPedals_GetSecondaryPedalPercentage(accelerator_pedals));

    // Heartbeat Communication
    App_CanTx_FSM_Vitals_Heartbeat_Set(true);
    if (App_CanRx_BMS_Vitals_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Vitals_Heartbeat_Update(false);
    }

    const bool hb_monitor_status_ok = App_SharedHeartbeatMonitor_Tick(hb_monitor);
    App_CanAlerts_SetFault(FSM_FAULT_MISSING_HEARTBEAT, !hb_monitor_status_ok);
    if (!hb_monitor_status_ok)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    App_AcceleratorPedals_Broadcast(world);
    App_Brake_Broadcast(world);
    App_Coolant_Broadcast(world);
    App_Steering_Broadcast(world);
    App_Wheels_Broadcast(world);
}
