#define MAX_NAME_SIZE 16 // taken from shared app_stateMachine.h

typedef struct {
    char name[MAX_NAME_SIZE];
    void (*run_on_entry)();
    void (*run_on_exit)();
    void (*run_on_tick)(); // TV State machine is only in 100Hz tasks so no need for 1Hz task
}TVinternalState;

/**
 * Create a state machine with the given world
 * @param state_machine The state machine to initialize
 * @param initial_state The initial state to start the state machine in
 */
void app_TVstateMachine_init(const TVinternalState *initial_state);

/**
 * Get the currently running state in the given state machine
 * @param state_machine The state machine to get the currently running state
 *                      from
 * @return The currently running state from the given state machine
 */
const TVinternalState *app_TVstateMachine_getCurrentState(void);

/**
 * Set the next state the state machine should go to
 * @param state_machine The state machine to set the next state on
 * @param next_state The next state
 */
void app_TVstateMachine_setNextState(const TVinternalState *next_state);

/**
 * Tick the 100Hz function of the given state machine
 * @param state_machine The state machine to tick
 */
void app_TVstateMachine_tick100Hz(void);