#pragma once

#include "App_SharedStateMachine.h"

/**
 * Get a pointer to the Standby state.
 * @return A pointer to the Standby state. THIS SHOULD NOT BE MODIFIED
 */
const struct State *App_GetStandbyState(void);