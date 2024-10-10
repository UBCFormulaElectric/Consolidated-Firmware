#pragma once

#include "app_stateMachine.h"

/**
 * Get a pointer to the Fault State.
 * @return A pointer to the Fault State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_faultState_get(void);
