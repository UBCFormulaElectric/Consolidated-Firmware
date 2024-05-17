#pragma once

#include "app_stateMachine.h"
#include "app_accumulator.h"

// Ignore the charger fault signal for the first 500 cycles (5 seconds)
#define CYCLES_TO_IGNORE_CHGR_FAULT (500U)

// Time to allow charger to disable before opening airs
#define CHARGER_SHUTDOWN_TIMEOUT (100U)
#define PREMATURE_EXIT_FILTER_THRESHOLD (20U)

// Setting these to testing, will need to be changed later.
#define INITIAL_CHARGING_VOLTAGE (330.0f)
#define INITIAL_CHARGING_CURRENT (15.0f)
#define INITIAL_MAX_MAINS_CURRENT (15.0f)

/**
 * Get a pointer to the Charge State.
 * @return A pointer to the Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_chargeState_get(void);
