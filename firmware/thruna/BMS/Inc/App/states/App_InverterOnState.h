#pragma once

#include "App_SharedStateMachine.h"

/**
 * Get a pointer to the Inverter State.
 * @return A pointer to the Inverter State.
 */
const struct State *App_GetInverterOnState(void);

/**
 * Reset hasTimePassed bool
 */
void App_InverterOnState_Init(void);