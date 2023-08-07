#pragma once

#include "App_SharedStateMachine.h"
#include "states/App_DriveState.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"
#include "states/App_AllStates.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"

/**
 * Get a pointer to the Pre-Charge State.
 * @return A pointer to the Pre-Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const struct State *App_GetPreChargeState(void);
