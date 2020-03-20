#pragma once

#include <stdbool.h>

// Application specific configuration options.
#include "App/States/App_SharedStateConfig.h"

// Check all the required application specific macros have been defined in
// <App_SharedStateConfig.h>.
#ifndef MAX_NUM_OF_STATE_TABLES
#error Missing definition: MAX_NUM_OF_STATE_TABLES must be defined in App_SharedStateConfig.h
#endif

#ifndef MAX_NUM_OF_STATES_PER_STATE_TABLE
#error Missing definition: MAX_NUM_OF_STATES_PER_STATE_TABLE must be defined in App_SharedStateConfig.h.
#endif

// Anonymous type by which states are referenced.
typedef void *StateHandle_t;

// Anonymous type by which state tables are referenced.
typedef void *StateTableHandle_t;

/**
 * Allocate storage for a state table.
 * @note This allocation uses statically-allocated memory, rather than malloc(),
 *       so there is no need to free() the pointer. The maximum number of state
 *       tables that can be allocated is specified in MAX_NUM_OF_STATE_TABLES.
 * @return Handle to the allocated state table
 */
StateTableHandle_t App_SharedState_AllocStateTable(void);

/**
 * Add a state to the state table (if there is space).
 * @param state_table_handle: Handle to the state table
 * @param state_name: Name of the state to add
 * @param run_on_entry: Function to run as we enter the state
 * @param run_on_exit: Function to run as we exit the state
 * @param run_state_action: Function to run during each state machine tick
 * @return Handle to the state we just added
 */
StateHandle_t App_SharedState_AddStateToStateTable(
    StateTableHandle_t state_table_handle,
    char *             state_name,
    void (*run_on_entry)(void),
    void (*run_on_exit)(void),
    void (*run_state_action)(void));

/**
 * Check if a state is in a state table.
 * @param state_table_handle: Handle to the state table being searched in
 * @param state_handle: Handle to the state being searched for
 * @return True if the state handle was found, false otherwise.
 */
bool App_SharedState_IsStateInStateTable(
    StateTableHandle_t state_table_handle,
    StateHandle_t      state);

/**
 * Run the on-entry function of a state handle.
 * @param state_handle: Handle to the subject state
 */
void App_SharedState_RunOnEntry(StateHandle_t state_handle);

/**
 * Run the on-exit function of a state handle.
 * @param state_handle: Handle to the subject state
 */
void App_SharedState_RunOnExit(StateHandle_t state_handle);

/**
 * Run the state action of a state handle.
 * @param state_handle: Handle to the subject state
 */
void App_SharedState_RunStateAction(StateHandle_t state_handle);

/**
 * Set the next state handle of a state handle.
 * @param state_handle: Handle to the subject state
 * @param next_state_handle: Handle to the next state
 */
void App_SharedState_SetNextState(
    StateHandle_t state_handle,
    StateHandle_t next_state_handle);

/**
 * Get the next state handle of a state handle.
 * @param state_handle: Handle to the subject state
 * @return Handle to the next state
 */
StateHandle_t App_SharedState_GetNextState(StateHandle_t state_handle);
