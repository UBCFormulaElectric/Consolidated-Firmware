#pragma once

#include <stdbool.h>

typedef struct State     State_t;
typedef struct StatePool StateTable_t;

/**
 *
 * @return
 */
StateTable_t *App_SharedState_AllocStateTable(void);

/**
 *
 * @param state_table
 * @param name
 * @param run_on_entry
 * @param run_on_exit
 * @param run_state_action
 * @return
 */
State_t *App_SharedState_AddStateToStateTable(
    StateTable_t *state_table,
    char *        name,
    void (*run_on_entry)(void),
    void (*run_on_exit)(void),
    void (*run_state_action)(void));

/**
 *
 * @param state_table
 * @param state
 * @return
 */
bool App_SharedState_IsStateInStateTable(
    StateTable_t *state_table,
    State_t *     state);

/**
 *
 * @param state:
 */
void App_SharedState_RunOnEntry(State_t *state);

/**
 *
 * @param state:
 */
void App_SharedState_RunOnExit(State_t *state);

/**
 *
 * @param state:
 */
void App_SharedState_RunStateAction(State_t *state);

/**
 *
 * @param state:
 * @param next_state:
 */
void App_SharedState_SetNextState(State_t *state, State_t *next_state);

/**
 * @param state:
 */
State_t *App_SharedState_GetNextState(State_t *state);
