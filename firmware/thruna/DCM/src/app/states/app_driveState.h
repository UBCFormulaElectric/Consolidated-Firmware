#pragma once

#include "app_stateMachine.h"

/**
 * Get a pointer to the Drive state.
 * @return A pointer to the Drive state. THIS SHOULD NOT BE MODIFIED
 */
const struct State *app_driveState_get(void);
