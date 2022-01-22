#include "states/App_AllStates.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence =
        App_DcmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx      = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx      = App_DcmWorld_GetCanRx(world);
    struct BrakeLight *       brake_light = App_DcmWorld_GetBrakeLight(world);
    double                    torque_command =
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_decode(
            App_CanTx_GetPeriodicSignal_TORQUE_COMMAND_INVL(can_tx));

    bool is_brake_actuated =
        App_CanRx_FSM_BRAKE_GetSignal_BRAKE_IS_ACTUATED(can_rx);
    bool is_regen_active = torque_command < 0.0;

    App_BrakeLight_SetLightStatus(
        brake_light, is_brake_actuated, is_regen_active);
}
