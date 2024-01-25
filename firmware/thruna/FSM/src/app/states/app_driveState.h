#pragma once

#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"

/**
 * Get a pointer to the Drive STATE
 * @return A pointer to the Drive STATE
 */
const struct State * app_driveState_get(void);
