#pragma once

#include "app_stateMachine.h"

/**
 * Get a pointer to the Init State.
 * @return A pointer to the Init State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *app_initState_get(void);
