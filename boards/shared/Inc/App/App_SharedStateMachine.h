#pragma once

#include "App_StateMachineConfig.h"

#define MAX_STATE_NAME_LENGTH 16

struct StateMachine;
struct State
{
    // A newline terminated string containing the name of this state
    char name[MAX_STATE_NAME_LENGTH];

    void (*run_on_enter)(struct StateMachine *state_machine);
    void (*run_on_tick)(struct StateMachine *state_machine);
    void (*run_on_exit)(struct StateMachine *state_machine);
};

/**
 * Create a state machine with the given world
 * @param world A world that will be used by the state machine for all of it's
 *              interactions with the external world.
 * @param initial_state The initial state to start the state machine in
 * @return A created state machine, or NULL if creation was unsuccessful
 */
struct StateMachine *App_SharedStateMachine_Create(
    struct World *world,
    struct State *initial_state);

/**
 * Get the currently running state in the given state machine
 * @param state_machine The state machine to get the currently running state
 *                      from
 * @return The currently running state from the given state machine
 */
struct State *
    App_SharedStateMachine_GetCurrentState(struct StateMachine *state_machine);

/**
 * Transition the given state machine to a new state
 * @param state_machine The state machine to transition to a new state
 * @param new_state The new state to transition the state machine to
 */
void App_SharedStateMachine_TransitionState(
    struct StateMachine *state_machine,
    struct State *       new_state);

// TODO: better jdoc here
/**
 * Get the world from the given state machine
 * @param state_machine The state machine to get the world from
 * @return The world from the given state machine
 */
struct World *
    App_SharedStateMachine_GetWorld(struct StateMachine *state_machine);

/**
 * Tick the given state machine
 *
 * @param state_machine The state machine to tick
 */
void App_SharedStateMachine_Tick(struct StateMachine *state_machine);
