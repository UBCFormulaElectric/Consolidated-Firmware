#pragma once

#include "App_SharedStateMachine.h"

/**
 * Get a pointer to the Fault State.
 * @return A pointer to the Fault State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State* app_faultState_get(void);
