#pragma once

#include <stdbool.h>

// Application specific configuration options.
#include "App_SharedStateConfig.h"

// Check all the required application specific macros have been defined.
// These macros are application specific and are defined within
// App_SharedStateConfig.h
#ifndef MAX_NUM_OF_STATE_TABLES
#error Missing definition: MAX_NUM_OF_STATE_TABLES must be defined in App_SharedStateConfig.h
#endif

#ifndef MAX_NUM_OF_STATES_PER_STATE_TABLE
#error Missing definition: MAX_NUM_OF_STATES_PER_STATE_TABLE must be defined in App_SharedStateConfig.h.
#endif

typedef struct State      State_t;
typedef struct StateTable StateTable_t;

/**
 * Allocate storage for a state table.
 * @note This allocation uses statically-allocated memory, rather than malloc(),
 *       so there is no need to free() the pointer. The maximum number of state
 *       tables that can be allocated is specified in MAX_NUM_OF_STATE_TABLES.
 * @return Pointer to an allocated state table
 */
StateTable_t *App_SharedState_AllocStateTable(void);

/**
 * Add a state to the state table (if there is space).
 * @param state_table: State table
 * @param state_name: Name of the state to add
 * @param run_on_entry: Function to run as we enter the state
 * @param run_on_exit: Function to run as we exit the state
 * @param run_state_action: Function to run during each state machine tick
 * @return Pointer to the state we just added
 */
State_t *App_SharedState_AddStateToStateTable(
    StateTable_t *state_table,
    char *        state_name,
    void (*run_on_entry)(void),
    void (*run_on_exit)(void),
    void (*run_state_action)(void));

/**
 * Check if a state is in a state table.
 * @param state_table: State table to search in
 * @param state: State to search for
 * @return True if state was found, false otherwise.
 */
bool App_SharedState_IsStateInStateTable(
    StateTable_t *state_table,
    State_t *     state);

/**
 * Run the on-entry function for a given state.
 * @param state: State
 */
void App_SharedState_RunOnEntry(State_t *state);

/**
 * Run the on-exit function for a given state.
 * @param state: State
 */
void App_SharedState_RunOnExit(State_t *state);

/**
 * Run the state action for a given state.
 * @param state: State
 */
void App_SharedState_RunStateAction(State_t *state);

/**
 * Set the next state value of a given state.
 * @param state: State
 * @param next_state: Next state
 */
void App_SharedState_SetNextState(State_t *state, State_t *next_state);

/**
 * Get the next state value of a given state.
 * @param state: State
 * @return Next state
 */
State_t *App_SharedState_GetNextState(State_t *state);
