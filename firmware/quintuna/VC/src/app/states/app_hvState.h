#pragma once

#include "app_stateMachine.h"

extern State hv_state;
/**
 * Get a pointer to the Drive State.
 * @return A pointer to the Drive State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_hvState_get(void);
