#pragma once

#include "App_SharedStateMachine.h"
#include "App_Accumulator.h"

/**
 * Get a pointer to the Charge State.
 * @return A pointer to the Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *App_GetChargeState(void);