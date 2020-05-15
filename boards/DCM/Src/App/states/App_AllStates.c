#include "App_SharedMacros.h"
#include "states/App_AllStates.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx      = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx      = App_DcmWorld_GetCanRx(world);
    struct BrakeLight *       brake_light = App_DcmWorld_GetBrakeLight(world);

    bool is_brake_actuated =
        App_CanRx_FSM_BRAKE_GetSignal_BRAKE_IS_ACTUATED(can_rx);
    bool is_regen_active =
        App_CanTx_GetPeriodicSignal_TORQUE_REQUEST(can_tx) < 0.0f;

    App_BrakeLight_SetLightStatus(
        brake_light, is_brake_actuated, is_regen_active);
}
