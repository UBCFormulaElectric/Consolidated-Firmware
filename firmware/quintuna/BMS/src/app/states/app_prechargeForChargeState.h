#pragma once

#include "app_stateMachine.h"

/**
 * Get a pointer to the Pre-Charge State for charging.
 * @return A pointer to the Pre-Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_prechargeForChargeState_get(void);
