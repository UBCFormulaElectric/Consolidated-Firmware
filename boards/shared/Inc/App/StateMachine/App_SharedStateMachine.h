#pragma once

// Application specific configuration options.
#include "StateMachine/App_SharedStateMachineConfig.h"

// Check all the required application specific macros have been defined in
// <App_SharedStateMachineConfig.h>.
#ifndef MAX_NUM_OF_STATE_MACHINES
#error Missing definition: MAX_NUM_OF_STATE_MACHINES must be defined in App_SharedStateMachineConfig.h.
#endif

// The state machine library relies on the the state library
#include "StateMachine/App_SharedState.h"

// Anonymous type by which state machines are referenced.
typedef void *StateMachineHandle_t;

/**
 * Allocate storage for a state table.
 * @note This allocation uses statically-allocated memory, rather than malloc(),
 *       so there is no need to free() the pointer. The maximum number of state
 *       tables that can be allocated is specified in MAX_NUM_OF_STATE_MACHINES.
 * @return Handle to an allocated state machine
 */
StateMachineHandle_t App_SharedStateMachine_Alloc(void);

/**
 * Add a state to state machine.
 * @param state_machine: State machine
 * @param state_name: Name of state to add
 * @param run_on_entry: Function to run as we enter the state
 * @param run_on_exit: Function to run as we exit the state
 * @param run_state_action: Function to run during each state machine tick
 * @return Handle to the state we just added
 */
StateHandle_t App_SharedStateMachine_AddState(
    StateMachineHandle_t state_machine,
    char *               state_name,
    void (*run_on_entry)(void),
    void (*run_on_exit)(void),
    void (*run_state_action)(void));

/**
 * Initialize a state machine by specifying the initial state to start in.
 * @param name: Name of the state machine
 * @param state_machine: State machine
 * @param initial_state: Initial state of the state machine
 */
void App_SharedStateMachine_Init(
    const char *         name,
    StateMachineHandle_t state_machine,
    StateHandle_t        initial_state);

/**
 * Run a single tick of the state machine.
 * @param state_machine: State machine
 */
void App_SharedStateMachine_Tick(StateMachineHandle_t state_machine);

/**
 * Get the current state of a state machine.
 * @param state_machine_handle: Handle to the subject state machine.
 * @return Handle to the current state of the subject state machine.
 */
StateHandle_t App_SharedStateMachine_GetCurrentState(
    StateMachineHandle_t state_machine_handle);
