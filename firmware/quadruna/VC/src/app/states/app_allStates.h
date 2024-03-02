#pragma once

#include "app_stateMachine.h"

/**
 * On-tick 1Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void app_allStates_runOnTick1Hz(void);

/**
 * On-tick 100Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void app_allStates_runOnTick100Hz(void);
