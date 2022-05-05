#include "states/App_SharedStates.h"

void App_SharedStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct DcmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_DcmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

void App_SharedStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct DcmWorld *         world             = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx            = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx            = App_DcmWorld_GetCanRx(world);
    struct BrakeLight *       brake_light       = App_DcmWorld_GetBrakeLight(world);
    struct InverterSwitches * inverter_switches = App_DcmWorld_GetInverterSwitches(world);

    // Only need single torque command since same command sent to both inverters
    const bool is_regen_active = App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
                                     App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx)) < 0.0f;

    App_BrakeLight_SetLightStatus(
        brake_light, App_CanRx_FSM_BRAKE_GetSignal_BRAKE_IS_ACTUATED(can_rx), is_regen_active);
    App_CanTx_SetPeriodicSignal_RIGHT_INVERTER_SWITCH(can_tx, App_InverterSwitches_IsRightOn(inverter_switches));
    App_CanTx_SetPeriodicSignal_LEFT_INVERTER_SWITCH(can_tx, App_InverterSwitches_IsLeftOn(inverter_switches));
}
