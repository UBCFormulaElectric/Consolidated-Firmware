#pragma once

namespace app
{
struct State
{
    // A newline terminated string containing the name of this state
    const char *name;

    void (*run_on_entry)();
    void (*run_on_tick_1Hz)();
    void (*run_on_tick_100Hz)();
    void (*run_on_exit)();
};

namespace StateMachine
{
    /**
     * Create a state machine with the given world
     * @param state_machine The state machine to initialize
     * @param initial_state The initial state to start the state machine in
     */
    // void app_stateMachine_init(const State *initial_state);
    void init(const State *initial_state);

    /**
     * Get the currently running state in the given state machine
     * @param state_machine The state machine to get the currently running state
     *                      from
     * @return The currently running state from the given state machine
     */
    const State *get_current_state();

    /**
     * Set the next state the state machine should go to
     * @param state_machine The state machine to set the next state on
     * @param next_state The next state
     */
    void set_next_state(const State *next_state);

    /**
     * Tick the 1hz function of the given state machine
     * @param state_machine The state machine to tick
     */
    void tick1Hz();

    /**
     * Tick the 100Hz function of the given state machine
     * @param state_machine The state machine to tick
     */
    void tick100Hz();
} // namespace StateMachine
} // namespace app