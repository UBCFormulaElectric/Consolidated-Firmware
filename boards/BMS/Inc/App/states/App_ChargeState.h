#pragma once

#include "App_SharedStateMachine.h"
#include "App_Accumulator.h"
// 0.05C is standard for a boundary to consider full charge
#define CURRENT_AT_MAX_CHARGE (0.05f * C_RATE_TO_AMPS);
/**
 * Get a pointer to the Charge State.
 * @return A pointer to the Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *App_GetChargeState(void);
