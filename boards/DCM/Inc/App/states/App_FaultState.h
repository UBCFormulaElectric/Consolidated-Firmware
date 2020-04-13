#pragma once

#include "App_SharedStateMachine.h"

/**
 * Get a pointer to the Fault state.
 * @return A pointer to the Fault state. THIS SHOULD NOT BE MODIFIED
 */
const struct State *App_GetFaultState(void);
