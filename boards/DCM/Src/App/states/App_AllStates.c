#include "states/App_FaultState.h"
#include "states/App_AllStates.h"

static void App_SendAndReceiveHeartbeat(
    struct ErrorTable *       error_table,
    struct HeartbeatMonitor * hb_monitor,
    struct DcmCanTxInterface *can_tx,
    struct DcmCanRxInterface *can_rx)
{
    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);
    if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }

    const bool is_missing_hb = !App_SharedHeartbeatMonitor_Tick(hb_monitor);
    App_CanTx_SetPeriodicSignal_MISSING_HEARTBEAT(can_tx, is_missing_hb);
    App_SharedErrorTable_SetError(error_table, DCM_AIR_SHUTDOWN_MISSING_HEARTBEAT, is_missing_hb);
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct DcmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_DcmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    bool                      status            = true;
    struct DcmWorld *         world             = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx            = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx            = App_DcmWorld_GetCanRx(world);
    struct BrakeLight *       brake_light       = App_DcmWorld_GetBrakeLight(world);
    struct InverterSwitches * inverter_switches = App_DcmWorld_GetInverterSwitches(world);
    struct HeartbeatMonitor * hb_monitor        = App_DcmWorld_GetHeartbeatMonitor(world);
    struct ErrorTable *       error_table       = App_DcmWorld_GetErrorTable(world);

    App_SendAndReceiveHeartbeat(error_table, hb_monitor, can_tx, can_rx);

    // Only need single torque command since same command sent to both inverters
    const bool is_regen_active = App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
                                     App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx)) < 0.0f;
    App_BrakeLight_SetLightStatus(
        brake_light, App_CanRx_FSM_BRAKE_GetSignal_BRAKE_IS_ACTUATED(can_rx), is_regen_active);
    App_CanTx_SetPeriodicSignal_RIGHT_INVERTER_SWITCH(can_tx, App_InverterSwitches_IsRightOn(inverter_switches));
    App_CanTx_SetPeriodicSignal_LEFT_INVERTER_SWITCH(can_tx, App_InverterSwitches_IsLeftOn(inverter_switches));

    if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table) || App_HasInverterFault(can_rx))
    {
        status = false;
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    return status;
}
