#include <stddef.h>
#include "SharedAssert.h"
#include "App_SharedStateMachine.h"

typedef struct StateTable StateTable_t;

#define MAX_STATE_MACHINE_NAME_LENGTH 16
typedef struct StateMachine
{
    // Is this state machine ready to be used?
    bool initialized;
    // State machine name (For debugging only)
    char name[MAX_STATE_MACHINE_NAME_LENGTH];
    // Previous state of the state machine (For debugging only)
    State_t *previous_state;
    // Current state of the state machine
    State_t *current_state;
    // Possibles states in this state machine
    StateTable_t *state_table;
} StateMachine_t;

StateMachine_t *App_SharedStateMachine_Alloc(const char *name)
{
    shared_assert(name != NULL);

    static StateMachine_t state_machines[MAX_NUM_OF_STATE_MACHINES];
    static size_t         alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_STATE_MACHINES);

    StateMachine_t *state_machine = &state_machines[alloc_index++];

    state_machine->state_table = App_SharedState_AllocStateTable();

    return state_machine;
}

State_t *App_SharedStateMachine_AddState(
    StateMachine_t *state_machine,
    char *          state_name,
    void (*run_on_entry)(void),
    void (*run_on_exit)(void),
    void (*run_state_action)(void))
{
    shared_assert(state_machine != NULL);
    shared_assert(state_name != NULL);
    shared_assert(run_on_entry != NULL);
    shared_assert(run_on_exit != NULL);
    shared_assert(run_state_action != NULL);

    return App_SharedState_AddStateToStateTable(
        state_machine->state_table, state_name, run_on_entry, run_on_exit,
        run_state_action);
}

void App_SharedStateMachine_Init(
    StateMachine_t *state_machine,
    State_t *       initial_state)
{
    shared_assert(state_machine != NULL);
    shared_assert(initial_state != NULL);

    // Check if the initial state has already been added
    bool found = App_SharedState_IsStateInStateTable(
        state_machine->state_table, initial_state);

    // The state machine can't run if the initial state hasn't been added to the
    // state machine.
    shared_assert(found == true);

    state_machine->current_state = initial_state;
    state_machine->initialized   = true;
}

void App_SharedStateMachine_Tick(StateMachine_t *state_machine)
{
    shared_assert(state_machine != NULL);
    shared_assert(state_machine->initialized == true);

    // Run the state action associated with the current state
    App_SharedState_RunStateAction(state_machine->current_state);

    // Check if the state action caused a state transition
    State_t *next_state =
        App_SharedState_GetNextState(state_machine->current_state);
    if (next_state != NULL)
    {
        // Record how the state transition happened
        state_machine->previous_state = state_machine->current_state;

        // Clear next_state field
        App_SharedState_SetNextState(state_machine->current_state, NULL);

        // Run on-exit function before going to next state
        App_SharedState_RunOnExit(state_machine->current_state);

        // Go to next state
        App_SharedState_SetNextState(state_machine->current_state, next_state);
        App_SharedState_RunOnEntry(state_machine->current_state);
    }
}
