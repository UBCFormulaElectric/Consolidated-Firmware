#include "states/App_AllStates.h"

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_FsmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}
