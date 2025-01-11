#pragma once

#include "app_stateMachine.h"

#define BUZZER_ON_DURATION_MS 2000

/**
 * Get a pointer to the Drive State.
 * @return A pointer to the Drive State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_driveState_get(void);
