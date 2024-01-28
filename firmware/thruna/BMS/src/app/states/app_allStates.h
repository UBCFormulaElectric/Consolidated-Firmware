#pragma once

#include "stdbool.h"
#include "states/app_driveState.h"
#include "states/app_initState.h"
#include "states/app_prechargeState.h"
#include "states/app_chargeState.h"
#include "states/app_balancingState.h"
#include "states/app_faultState.h"
#include "app_globals.h"
#include "app_stateMachine.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canRx.h"

/**
 * On-tick 1Hz function for every state in the given state machine
 */
void app_allStates_runOnTick1Hz(void);

/**
 * On-tick 100Hz function for every state in the given state machine
 * @return True if the next state is not the fault state, otherwise false
 */
bool app_allStates_runOnTick100Hz(void);
