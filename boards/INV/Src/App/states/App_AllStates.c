#include "states/App_AllStates.h"
#include "App_SharedMacros.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence =
        App_InvWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx = App_InvWorld_GetCanTx(world);
    struct InvCanRxInterface *can_rx = App_InvWorld_GetCanRx(world);
    UNUSED(can_rx);
    UNUSED(can_tx);
}
