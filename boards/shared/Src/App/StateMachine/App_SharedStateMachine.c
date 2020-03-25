#include <string.h>

#include "App_SharedAssert.h"
#include "StateMachine/App_SharedStateMachine.h"

#define MAX_STATE_MACHINE_NAME_LENGTH 16
typedef struct StateMachine
{
    // Is this state machine ready to be used?
    bool initialized;
    // State machine name (For debugging only)
    char name[MAX_STATE_MACHINE_NAME_LENGTH];
    // Previous state of the state machine (For debugging only)
    StateHandle_t previous_state;
    // Current state of the state machine
    StateHandle_t current_state;
    // Possibles states in this state machine
    StateTableHandle_t state_table_handle;
} StateMachine_t;

// Return a pointer to the underlying state machine type
#define prvGetStateMachineFromHandle(handle) (StateMachine_t *)(handle)

StateMachineHandle_t App_SharedStateMachine_Alloc(void)
{
    static StateMachine_t state_machines[MAX_NUM_OF_STATE_MACHINES];
    static size_t         alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_STATE_MACHINES);

    StateMachine_t *state_machine = &state_machines[alloc_index++];

    state_machine->initialized        = false;
    state_machine->previous_state     = NULL;
    state_machine->current_state      = NULL;
    state_machine->state_table_handle = App_SharedState_AllocStateTable();

    return (StateMachineHandle_t)state_machine;
}

StateHandle_t App_SharedStateMachine_AddState(
    StateMachineHandle_t state_machine_handle,
    char *               state_name,
    void (*run_on_entry)(void),
    void (*run_on_exit)(void),
    void (*run_state_action)(void))
{
    shared_assert(state_machine_handle != NULL);
    shared_assert(state_name != NULL);
    shared_assert(run_on_entry != NULL);
    shared_assert(run_on_exit != NULL);
    shared_assert(run_state_action != NULL);

    StateMachine_t *state_machine =
        prvGetStateMachineFromHandle(state_machine_handle);

    return App_SharedState_AddStateToStateTable(
        state_machine->state_table_handle, state_name, run_on_entry,
        run_on_exit, run_state_action);
}

void App_SharedStateMachine_Init(
    const char *         name,
    StateMachineHandle_t state_machine_handle,
    StateHandle_t        initial_state)
{
    shared_assert(name != NULL);
    shared_assert(state_machine_handle != NULL);
    shared_assert(initial_state != NULL);

    StateMachine_t *state_machine =
        prvGetStateMachineFromHandle(state_machine_handle);

    // Our state machine doesn't really support the semantics of being
    // initialized more than one time.
    shared_assert(state_machine->initialized == false);
    shared_assert(state_machine->state_table_handle != NULL);

    // Check if the initial state has already been added
    bool found = App_SharedState_IsStateInStateTable(
        state_machine->state_table_handle, initial_state);

    // The state machine can't run if the initial state hasn't been added to the
    // state machine.
    shared_assert(found == true);

    strncpy(state_machine->name, name, MAX_STATE_MACHINE_NAME_LENGTH);
    state_machine->current_state = initial_state;

    // We need to manually call the first on-entry function
    App_SharedState_RunOnEntry(state_machine->current_state);

    state_machine->initialized = true;
}

void App_SharedStateMachine_Tick(StateMachineHandle_t state_machine_handle)
{
    shared_assert(state_machine_handle != NULL);

    StateMachine_t *state_machine =
        prvGetStateMachineFromHandle(state_machine_handle);

    shared_assert(state_machine->initialized == true);

    // Run the state action associated with the current state
    App_SharedState_RunStateAction(state_machine->current_state);

    // Check if the state action caused a state transition
    StateHandle_t next_state =
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
        state_machine->current_state = next_state;
        App_SharedState_RunOnEntry(state_machine->current_state);
    }
}

StateHandle_t App_SharedStateMachine_GetCurrentState(
    StateMachineHandle_t state_machine_handle)
{
    shared_assert(state_machine_handle != NULL);

    StateMachine_t *state_machine =
        prvGetStateMachineFromHandle(state_machine_handle);

    shared_assert(state_machine->initialized == true);

    return (StateHandle_t)state_machine->current_state;
}
