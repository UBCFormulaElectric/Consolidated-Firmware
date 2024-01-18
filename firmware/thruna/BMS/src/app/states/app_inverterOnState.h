#pragma once

#include "app_stateMachine.h"

/**
 * Get a pointer to the Inverter State.
 * @return A pointer to the Inverter State.
 */
const struct State *app_inverterOnState_get(void);

/**
 * Reset hasTimePassed bool
 */
void app_inverterOnState_init(void);
