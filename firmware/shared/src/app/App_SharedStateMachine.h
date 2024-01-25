#pragma once

#include "configs/App_SharedStateMachineConfig.h"

#define MAX_STATE_NAME_LENGTH 16

#ifndef World
#error "Please define the 'World' type"
#endif

struct StateMachine;
struct State
{
    // A newline terminated string containing the name of this state
    char name[MAX_STATE_NAME_LENGTH];

    void (*run_on_entry)(struct StateMachine *state_machine);
    void (*run_on_tick_1Hz)(struct StateMachine *state_machine);
    void (*run_on_tick_100Hz)(struct StateMachine *state_machine);
    void (*run_on_exit)(struct StateMachine *state_machine);
};

/**
 * Create a state machine with the given world
 * @param world A world that will be used by the state machine for all of it's
 *              interactions with the external world. The created state
 *              machine will *not* take ownership of this world, and it must
 *              be kept alive for the lifetime of the created state machine
 * @param initial_state The initial state to start the state machine in
 * @return A created state machine, or NULL if creation was unsuccessful
 */
struct StateMachine *App_SharedStateMachine_Create(struct World *world, const struct State *initial_state);

/**
 * Deallocate the memory used by the given state machine
 * @param state_machine The state machine to deallocate
 */
void App_SharedStateMachine_Destroy(struct StateMachine *state_machine);

/**
 * Get the currently running state in the given state machine
 * @param state_machine The state machine to get the currently running state
 *                      from
 * @return The currently running state from the given state machine
 */
const struct State *App_SharedStateMachine_GetCurrentState(const struct StateMachine *state_machine);

/**
 * Set the next state the state machine should go to
 * @param state_machine The state machine to set the next state on
 * @param next_state The next state
 */
void App_SharedStateMachine_SetNextState(struct StateMachine *state_machine, const struct State *next_state);

/**
 * Get the world from the given state machine
 * @param state_machine The state machine to get the world from
 * @return The world from the given state machine
 */
struct World *App_SharedStateMachine_GetWorld(const struct StateMachine *state_machine);

/**
 * Tick the 1hz function of the given state machine
 * @param state_machine The state machine to tick
 */
void App_SharedStateMachine_Tick1Hz(struct StateMachine *state_machine);

/**
 * Tick the 100Hz function of the given state machine
 * @param state_machine The state machine to tick
 */
void App_SharedStateMachine_Tick100Hz(struct StateMachine *state_machine);
