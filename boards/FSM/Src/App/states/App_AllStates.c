#include "states/App_AllStates.h"
#include "App_SharedConstants.h"

#define TORQUE_LIMIT_OFFSET_NM (5.0f)
#define MAX_TORQUE_PLAUSIBILITY_ERR_CNT (25) // 250 ms window

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanTxInterface *can_tx           = App_FsmWorld_GetCanTx(world);
    struct RgbLedSequence *   rgb_led_sequence = App_FsmWorld_GetRgbLedSequence(world);
    struct Brake *            brake            = App_FsmWorld_GetBrake(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE_OPEN_OC(can_tx, App_Brake_IsPressureSensorOpenCircuit(brake));
    App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE_OPEN_SC(can_tx, App_Brake_IsPressureSensorShortCircuited(brake));
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanTxInterface *can_tx      = App_FsmWorld_GetCanTx(world);
    struct FsmCanRxInterface *can_rx      = App_FsmWorld_GetCanRx(world);
    struct HeartbeatMonitor * hb_monitor  = App_FsmWorld_GetHeartbeatMonitor(world);
    static uint8_t            error_count = 0;

    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }

    // Check for torque plausibility
    float left_torque_req = App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
        App_CanRx_DCM_INVL_COMMAND_MESSAGE_GetSignal_TORQUE_COMMAND_INVL(can_rx));
    float right_torque_req = App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
        App_CanRx_DCM_INVR_COMMAND_MESSAGE_GetSignal_TORQUE_COMMAND_INVR(can_rx));
    float fsm_torque_limit = App_CanTx_GetPeriodicSignal_FSM_TORQUE_LIMIT(can_tx);
    if (left_torque_req > fsm_torque_limit || right_torque_req > fsm_torque_limit)
    {
        error_count++;
    }
    else
    {
        error_count = 0;
    }

    if (error_count == MAX_TORQUE_PLAUSIBILITY_ERR_CNT)
    {
        App_CanTx_SetPeriodicSignal_TORQUE_PLAUSIBILITY_CHECK_FAILED(can_tx, true);
    }

    // Broadcast a new FSM torque limit based on pedal percentage
    fsm_torque_limit = 0.01f * App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx) * MAX_TORQUE_REQUEST_NM +
                       TORQUE_LIMIT_OFFSET_NM;
    App_CanTx_SetPeriodicSignal_FSM_TORQUE_LIMIT(can_tx, fsm_torque_limit);

    App_CanTx_SetPeriodicSignal_MISSING_HEARTBEAT(can_tx, !App_SharedHeartbeatMonitor_Tick(hb_monitor));
}
