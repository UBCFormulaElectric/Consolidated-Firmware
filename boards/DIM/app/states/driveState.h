#pragma once

#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "App_SevenSegDisplays.h"
#include "App_SharedExitCode.h"

/**
 * Get a pointer to the Drive State.
 * @return A pointer to the Drive State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *driveState_getState(void);
