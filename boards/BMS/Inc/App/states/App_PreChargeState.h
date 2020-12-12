#pragma once

#include "App_SharedStateMachine.h"

/**
 * Get a pointer to the Pre-Charge State.
 * @return A pointer to the Pre-Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *App_GetPreChargeState(void);
