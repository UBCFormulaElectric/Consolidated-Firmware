#pragma once

#include "app_stateMachine.h"

#define INVERTER_BOOTUP_TIME_MS (200U)

/**
 * Get a pointer to the Inverter State.
 * @return A pointer to the Inverter State.
 */
const State *app_inverterOnState_get(void);
