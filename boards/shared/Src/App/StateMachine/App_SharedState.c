#include <string.h>
#include "Io/Io_SharedAssert.h"
#include "App/StateMachine/App_SharedState.h"

#define MAX_STATE_NAME_LENGTH 16
typedef struct State
{
    // State name (For debugging only)
    char name[MAX_STATE_NAME_LENGTH];
    // Function that runs as we enter this state
    void (*run_on_entry)(void);
    // Function that runs as we exit this state
    void (*run_on_exit)(void);
    // Function that runs at every 'tick' of the state machine
    void (*run_state_action)(void);
    // next_state == NULL means no state transition requested
    // next_state != NULL means state transition requested
    struct State *next_state;
} State_t;

// Return a pointer to the underlying state type
#define prvGetStateFromHandle(handle) (State_t *)(handle)

typedef struct StateTable
{
    // Count of many entries are occupied in the state table
    size_t current_count;
    // Storage for holding states in the table
    State_t states[MAX_NUM_OF_STATES_PER_STATE_TABLE];
} StateTable_t;

// Return a pointer to the underlying state table type
#define prvGetStateTableFromHandle(handle) (StateTable_t *)(handle)

/**
 * Get the next available entry (if any) in the state table.
 * @param state_table_handle: Handle to the state table
 * @return Pointer to the next available entry
 */
static StateHandle_t App_SharedState_GetNextFreeStateInStateTable(
    StateTableHandle_t state_table_handle);

static StateHandle_t App_SharedState_GetNextFreeStateInStateTable(
    StateTableHandle_t state_table_handle)
{
    shared_assert(state_table_handle != NULL);

    StateTable_t *state_table = prvGetStateTableFromHandle(state_table_handle);

    shared_assert(
        state_table->current_count < MAX_NUM_OF_STATES_PER_STATE_TABLE);

    return &state_table->states[state_table->current_count++];
}

StateTableHandle_t App_SharedState_AllocStateTable(void)
{
    static StateTable_t state_tables[MAX_NUM_OF_STATE_TABLES];
    static size_t       alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_STATE_TABLES);

    return (StateTableHandle_t)&state_tables[alloc_index++];
}

StateHandle_t App_SharedState_AddStateToStateTable(
    StateTableHandle_t state_table,
    char *             state_name,
    void (*run_on_entry)(void),
    void (*run_on_exit)(void),
    void (*run_state_action)(void))
{
    shared_assert(state_table != NULL);
    shared_assert(state_name != NULL);
    shared_assert(run_on_entry != NULL);
    shared_assert(run_on_exit != NULL);
    shared_assert(run_state_action != NULL);

    StateHandle_t state_handle =
        App_SharedState_GetNextFreeStateInStateTable(state_table);
    State_t *state = prvGetStateFromHandle(state_handle);

    state->run_on_entry     = run_on_entry;
    state->run_on_exit      = run_on_exit;
    state->run_state_action = run_state_action;
    state->next_state       = NULL;
    strncpy(state->name, state_name, MAX_STATE_NAME_LENGTH);

    return state;
}

bool App_SharedState_IsStateInStateTable(
    StateTableHandle_t state_table_handle,
    StateHandle_t      state_handle)
{
    shared_assert(state_table_handle != NULL);
    shared_assert(state_handle != NULL);

    StateTable_t *state_table = prvGetStateTableFromHandle(state_table_handle);
    State_t *     state       = prvGetStateFromHandle(state_handle);

    bool found = false;

    for (size_t i = 0; i < MAX_NUM_OF_STATES_PER_STATE_TABLE; i++)
    {
        if (memcmp(&state_table->states[i], state, sizeof(*state)) == 0)
        {
            found = true;
            break;
        }
    }

    return found;
}

void App_SharedState_RunOnEntry(StateHandle_t state_handle)
{
    shared_assert(state_handle != NULL);

    State_t *state = prvGetStateFromHandle(state_handle);
    state->run_on_entry();
}

void App_SharedState_RunOnExit(StateHandle_t state_handle)
{
    shared_assert(state_handle != NULL);

    State_t *state = prvGetStateFromHandle(state_handle);
    state->run_on_exit();
}

void App_SharedState_RunStateAction(StateHandle_t state_handle)
{
    shared_assert(state_handle != NULL);

    State_t *state = prvGetStateFromHandle(state_handle);
    state->run_state_action();
}

void App_SharedState_SetNextState(
    StateHandle_t state_handle,
    StateHandle_t new_state_handle)
{
    shared_assert(state_handle != NULL);

    State_t *state    = prvGetStateFromHandle(state_handle);
    state->next_state = prvGetStateFromHandle(new_state_handle);
}

StateHandle_t App_SharedState_GetNextState(StateHandle_t state_handle)
{
    shared_assert(state_handle != NULL);

    State_t *state = prvGetStateFromHandle(state_handle);
    return (StateHandle_t)state->next_state;
}
