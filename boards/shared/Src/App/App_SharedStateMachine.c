#include "App_SharedStateMachine.h"

// TODO: lots of checks that the `StateMachine` is never null....

#include <stdlib.h>

struct StateMachine
{
    struct State *next_state;
    struct State *current_state;
    struct World *world;
};

struct StateMachine *App_SharedStateMachine_Create(
    struct World *world,
    struct State *initial_state)
{
    struct StateMachine *state_machine = (struct StateMachine*)malloc(sizeof(struct StateMachine));

    state_machine->world = world;

    state_machine->current_state = initial_state;
    state_machine->next_state = initial_state;
    state_machine->current_state->run_on_enter(state_machine);

    return state_machine;
}

void App_SharedStateMachine_Destroy(struct StateMachine* state_machine){
    free(state_machine);
}

struct State *
    App_SharedStateMachine_GetCurrentState(struct StateMachine *state_machine)
{
    return state_machine->current_state;
}

void App_SharedStateMachine_SetNextState(
    struct StateMachine *state_machine,
    struct State *       next_state)
{
    state_machine->next_state = next_state;
}

struct World *
    App_SharedStateMachine_GetWorld(struct StateMachine *state_machine)
{
    return state_machine->world;
}

void App_SharedStateMachine_Tick(struct StateMachine *state_machine)
{
    // Check if we should transition states
    if (state_machine->next_state != state_machine->current_state){
        state_machine->current_state->run_on_exit(state_machine);
        state_machine->current_state = state_machine->next_state;
        state_machine->current_state->run_on_enter(state_machine);
    }

    // We assume the next time we tick we will continue in the current state,
    // unless told otherwise.
    state_machine->next_state = state_machine->current_state;

    state_machine->current_state->run_on_tick(state_machine);
}
