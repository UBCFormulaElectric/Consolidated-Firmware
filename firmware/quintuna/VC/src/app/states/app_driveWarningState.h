#pragma once

#include "app_stateMachine.h"


extern State driveWarning_state;
/**
 * Get a pointer to the Drive State.
 * @return A pointer to the Drive State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_driveWarningState_get(void);
