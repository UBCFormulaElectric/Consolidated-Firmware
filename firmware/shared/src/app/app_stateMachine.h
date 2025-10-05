#pragma once

typedef struct
{
    // A newline terminated string containing the name of this state
    const char *name;

    void (*run_on_entry)();
    void (*run_on_tick_100Hz)();
    void (*run_on_exit)();
} State;

/**
 * Create a state machine with the given world
 * @param initial_state The initial state to start the state machine in
 */
void app_stateMachine_init(const State *initial_state);

/**
 * Get the currently running state in the given state machine
 * @return The currently running state from the given state machine
 */
const State *app_stateMachine_getCurrentState(void);

/**
 * Set the next state the state machine should go to
 * @param state The next state
 */
void app_stateMachine_setNextState(const State *state);

/**
 * Tick the 100Hz function of the given state machine
 */
void app_stateMachine_tick100Hz(void);

/**
 * Tick to transition states to the next states
 */
void app_stateMachine_tickTransitionState(void);

void app_stateMachine_inverterFaultHandling(void);

#ifdef TARGET_TEST
void app_stateMachine_setCurrentState(const State *state);
#endif