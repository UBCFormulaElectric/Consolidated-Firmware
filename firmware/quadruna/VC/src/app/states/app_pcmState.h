#pragma once

#include "app_stateMachine.h"

/**
 * Get a pointer to the PCM State.
 * @return A pointer to the PCM State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_pcmState_get(void);