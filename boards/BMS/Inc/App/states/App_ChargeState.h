#pragma once

#include "App_SharedStateMachine.h"

/**
 * Get a pointer to the Charge State.
 * @return A pointer to the Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *App_GetChargeState(void);

//#define CURRENT_AT_MAX_CHARGE ;