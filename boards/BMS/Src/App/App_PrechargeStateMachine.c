#include <assert.h>
#include <stdlib.h>
#include "App_PrechargeStateMachine.h"

struct PreChargeStateMachine
{
    struct PreChargeState *current_state;
    struct PreChargeState *next_state;
};

struct PreChargeStateMachine *App_PreChargeStateMachine_Create(void)
{
    struct PreChargeStateMachine *state_machine =
        malloc(sizeof(struct PreChargeStateMachine));
    assert(state_machine != NULL);

    state_machine->current_state = App_PreChargeState_GetInitState();
    state_machine->next_state    = App_PreChargeState_GetInitState();

    return state_machine;
}

void App_PreChargeStateMachine_SetNextState(
    struct PreChargeSequence *const pre_charge_sequence,
    struct PreChargeState *const    next_state)
{
    struct PreChargeStateMachine *state_machine =
        App_PreChargeSequence_GetStateMachine(pre_charge_sequence);
    state_machine->next_state = next_state;
}

struct PreChargeState *App_PreChargeStateMachine_GetCurrentState(
    struct PreChargeStateMachine *const state_machine)
{
    return state_machine->current_state;
}

void App_PreChargeStateMachine_Tick(struct BmsWorld *const world)
{
    struct PreChargeSequence *pre_charge_sequence =
        App_BmsWorld_GetPreChargeSequence(world);
    struct PreChargeStateMachine *state_machine =
        App_PreChargeSequence_GetStateMachine(pre_charge_sequence);

    if (state_machine->current_state != state_machine->next_state)
    {
        state_machine->current_state = state_machine->next_state;
    }

    App_PreChargeState_RunOnTick(state_machine->current_state, world);
}
