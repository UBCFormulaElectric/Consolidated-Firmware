#pragma once

#define MAX_STATE_NAME_LENGTH 16

typedef struct
{
    // A newline terminated string containing the name of this state
    char name[MAX_STATE_NAME_LENGTH];

    void (*run_on_entry)();
    void (*run_on_tick_1Hz)();
    void (*run_on_tick_100Hz)();
    void (*run_on_exit)();
} State;

/**
 * Create a state machine with the given world
 * @param state_machine The state machine to initialize
 * @param initial_state The initial state to start the state machine in
 */
void app_stateMachine_init(const State *initial_state);

/**
 * Get the currently running state in the given state machine
 * @param state_machine The state machine to get the currently running state
 *                      from
 * @return The currently running state from the given state machine
 */
const State *app_stateMachine_getCurrentState(void);

/**
 * Set the next state the state machine should go to
 * @param state_machine The state machine to set the next state on
 * @param next_state The next state
 */
void app_stateMachine_setNextState(const State *next_state);

/**
 * Tick the 1hz function of the given state machine
 * @param state_machine The state machine to tick
 */
void app_stateMachine_tick1Hz(void);

/**
 * Tick the 100Hz function of the given state machine
 * @param state_machine The state machine to tick
 */
void app_stateMachine_tick100Hz(void);
