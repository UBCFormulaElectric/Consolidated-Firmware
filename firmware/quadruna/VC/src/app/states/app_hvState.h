#pragma once

#include "app_stateMachine.h"

/**
 * Get a pointer to the HV State.
 * @return A pointer to the HV State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_hvState_get(void);
