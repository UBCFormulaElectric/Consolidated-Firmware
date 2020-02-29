#pragma once

#include "App_SharedState.h"

typedef struct State        State_t;
typedef struct StateMachine StateMachine_t;

/**
 *
 * @param name
 * @return
 */
StateMachine_t *App_SharedStateMachine_Alloc(const char *name);

/**
 *
 * @param state_machine
 * @param name
 * @param run_on_entry
 * @param run_on_exit
 * @param run_state_action
 * @return
 */
State_t *App_SharedStateMachine_AddState(
    StateMachine_t *state_machine,
    char *          name,
    void (*run_on_entry)(void),
    void (*run_on_exit)(void),
    void (*run_state_action)(void));

/**
 * @param initial_state
 */
void App_SharedStateMachine_Init(
    StateMachine_t *state_machine,
    State_t *       initial_state);

/**
 * Update the state machine.
 * @param state_machine
 */
void App_SharedStateMachine_Update(StateMachine_t *state_machine);
