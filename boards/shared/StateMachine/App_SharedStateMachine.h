#pragma once

#include "App_SharedStateType.h"

/**
 * @param initial_state
 */
void App_SharedStateMachine_Init(struct State *initial_state);

/**
 * @brief Update the state machine.
 */
void App_SharedStateMachine_Tick(void);
