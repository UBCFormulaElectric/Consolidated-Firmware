#pragma once

#include "App_CanAlerts.h"
#include "App_CanRx.h"
#include "App_CanTx.h"
#include "App_SharedStateMachine.h"
#include "app_globals.h"
#include "states/app_balancingState.h"
#include "states/app_chargeState.h"
#include "states/app_driveState.h"
#include "states/app_faultState.h"
#include "states/app_initState.h"
#include "states/app_prechargeState.h"
#include "stdbool.h"

/**
 * On-tick 1Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void app_allStates_runOnTick1Hz(struct StateMachine* state_machine);

/**
 * On-tick 100Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 * @return True if the next state is not the fault state, otherwise false
 */
bool app_allStates_runOnTick100Hz(struct StateMachine* state_machine);
