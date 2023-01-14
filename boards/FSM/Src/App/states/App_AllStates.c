#include "states/App_AllStates.h"
#include "App_SharedConstants.h"

#define TORQUE_LIMIT_OFFSET_NM (5.0f)
#define MAX_TORQUE_PLAUSIBILITY_ERR_CNT (25) // 250 ms window

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_FsmWorld_GetRgbLedSequence(world);
    struct Brake *         brake            = App_FsmWorld_GetBrake(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE_OPEN_OC(can_tx,
    // App_Brake_IsPressureSensorOpenCircuit(brake));
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_BRAKE_PRESSURE_OPEN_SC(can_tx,
    // App_Brake_IsPressureSensorShortCircuited(brake));
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *         world              = App_SharedStateMachine_GetWorld(state_machine);
    struct HeartbeatMonitor * hb_monitor         = App_FsmWorld_GetHeartbeatMonitor(world);
    struct AcceleratorPedals *accelerator_pedals = App_FsmWorld_GetPappsAndSapps(world);
    static uint8_t            error_count        = 0;

    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    // TODO: JSONCAN -> if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    if (false)
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        // TODO: JSONCAN -> App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }

    // Check for torque plausibility
    // TODO: JSONCAN
    // float left_torque_req = (float)App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
    //     App_CanRx_DCM_INVL_COMMAND_MESSAGE_GetSignal_TORQUE_COMMAND_INVL(can_rx));
    // float right_torque_req = (float)App_CanMsgs_dcm_invr_command_message_torque_command_invr_decode(
    //     App_CanRx_DCM_INVR_COMMAND_MESSAGE_GetSignal_TORQUE_COMMAND_INVR(can_rx));

    float left_torque_req  = 0;
    float right_torque_req = 0;

    float fsm_torque_limit = 0; // TODO: JSONCAN -> App_CanTx_GetPeriodicSignal_FSM_TORQUE_LIMIT(can_tx);
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
        // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_TORQUE_PLAUSIBILITY_CHECK_FAILED(can_tx, true);
    }

    // Broadcast a new FSM torque limit based on pedal percentage
    // TODO: JSONCAN
    //    fsm_torque_limit = 0.01f * App_CanTx_GetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx) * MAX_TORQUE_REQUEST_NM
    //    +
    //                       TORQUE_LIMIT_OFFSET_NM;
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_FSM_TORQUE_LIMIT(can_tx, fsm_torque_limit);

    // Debug msgs, remove after testing
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_PAPPS(
    // can_tx, (uint16_t)App_AcceleratorPedals_GetPrimaryPedalPercentage(accelerator_pedals));
    // App_CanTx_SetPeriodicSignal_SAPPS(
    // can_tx, (uint16_t)App_AcceleratorPedals_GetSecondaryPedalPercentage(accelerator_pedals));

    // App_CanTx_SetPeriodicSignal_PAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, App());
    // App_CanTx_SetPeriodicSignal_SAPPS_MAPPED_PEDAL_PERCENTAGE(can_tx, Io_AcceleratorPedals_GetPapps());

    // App_CanTx_SetPeriodicSignal_MISSING_HEARTBEAT(can_tx, !App_SharedHeartbeatMonitor_Tick(hb_monitor));
}
