#pragma once

#include "app_stateMachine.h"

/**
 * Get a pointer to the Charge State.
 * @return A pointer to the Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_chargeState_get(void);
