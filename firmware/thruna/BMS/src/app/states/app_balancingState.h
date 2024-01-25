#pragma once

#include "App_SharedStateMachine.h"

/**
 * Get a pointer to the Balancing State.
 * @return A pointer to the Balancing State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State * app_balancingState_get(void);
