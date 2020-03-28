#include "App_SharedStateMachine.h"
struct StateMachine
{
    struct State *current_state;
    struct World *world;
};

struct StateMachine *
    App_SharedStateMachine_Create(struct World *world, struct State *initial_state)
{
    static struct StateMachine
                  state_machine_allocation_table[MAX_NUMBER_OF_STATE_MACHINES];
    static size_t curr_alloc_index = 0;

    // Make sure we're not going to run off the end of the allocation table
    if (curr_alloc_index >= MAX_NUMBER_OF_STATE_MACHINES)
    {
        return NULL;
    }

    struct StateMachine *state_machine =
        &state_machine_allocation_table[curr_alloc_index++];

    state_machine->world = world;

    state_machine->current_state = initial_state;
    state_machine->current_state->run_on_enter(state_machine);
}

void App_SharedStateMachine_TransitionState(
    struct StateMachine *state_machine,
    struct State *       new_state)
{
    state_machine->current_state->run_on_exit(state_machine);
    state_machine->current_state = new_state;
    state_machine->current_state->run_on_enter(state_machine);
}

struct World *App_SharedStateMachine_GetWorld(struct StateMachine *state_machine)
{
    return state_machine->world;
}

void App_SharedStateMachine_Tick(struct StateMachine *state_machine)
{
    state_machine->current_state->run_on_tick(state_machine);
}
