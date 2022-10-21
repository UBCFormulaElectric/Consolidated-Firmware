#pragma once

#include "App_SharedStateMachine.h"

/**
 * Get a pointer to the Default State.
 * @return A pointer to the Default State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *App_GetDefaultState(void);
