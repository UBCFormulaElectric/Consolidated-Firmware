#include "App_SharedStateMachine.h"
#include "App_SharedAssert.h"

#include <stdlib.h>

struct StateMachine
{
    const struct State *next_state;
    const struct State *current_state;
    struct World *      world;
};

struct StateMachine *App_SharedStateMachine_Create(
    struct World *      world,
    const struct State *initial_state)
{
    struct StateMachine *state_machine =
        (struct StateMachine *)malloc(sizeof(struct StateMachine));
    shared_assert(state_machine != NULL);

    state_machine->world = world;

    state_machine->current_state = initial_state;
    state_machine->next_state    = initial_state;
    state_machine->current_state->run_on_enter(state_machine);

    return state_machine;
}

void App_SharedStateMachine_Destroy(struct StateMachine *const state_machine)
{
    shared_assert(state_machine != NULL);
    free(state_machine);
}

const struct State *App_SharedStateMachine_GetCurrentState(
    const struct StateMachine *const state_machine)
{
    shared_assert(state_machine != NULL);
    return state_machine->current_state;
}

void App_SharedStateMachine_SetNextState(
    struct StateMachine *const state_machine,
    const struct State *const  next_state)
{
    shared_assert(state_machine != NULL);
    state_machine->next_state = next_state;
}

struct World *App_SharedStateMachine_GetWorld(
    const struct StateMachine *const state_machine)
{
    shared_assert(state_machine != NULL);
    return state_machine->world;
}

void App_SharedStateMachine_Tick(struct StateMachine *const state_machine)
{
    shared_assert(state_machine != NULL);
    // Check if we should transition states
    if (state_machine->next_state != state_machine->current_state)
    {
        state_machine->current_state->run_on_exit(state_machine);
        state_machine->current_state = state_machine->next_state;
        state_machine->current_state->run_on_enter(state_machine);
    }

    // We assume the next time we tick we will continue in the current state,
    // unless told otherwise.
    state_machine->next_state = state_machine->current_state;

    state_machine->current_state->run_on_tick(state_machine);
}
