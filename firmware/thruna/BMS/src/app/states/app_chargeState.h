#pragma once

#include "App_SharedStateMachine.h"
#include "app_accumulator.h"

// Ignore the charger fault signal for the first 500 cycles (5 seconds)
#define CYCLES_TO_IGNORE_CHGR_FAULT (500U)

// Time to allow charger to disable before opening airs
#define CHARGER_SHUTDOWN_TIMEOUT (100U)

/**
 * Get a pointer to the Charge State.
 * @return A pointer to the Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State* app_chargeState_get(void);
