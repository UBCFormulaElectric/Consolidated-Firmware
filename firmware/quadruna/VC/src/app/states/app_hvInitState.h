#pragma once

#include "app_stateMachine.h"

/**
 * Get a pointer to the HV Init State.
 * @return A pointer to the HV Init State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_hvInitState_get(void);
