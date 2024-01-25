#pragma once

#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"

/**
 * Get a pointer to the Drive State.
 * @return A pointer to the Drive State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *app_driveState_get(void);
