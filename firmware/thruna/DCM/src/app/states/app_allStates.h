#pragma once

#include <stdbool.h>
#include "App_SharedStateMachine.h"

/**
 * On-tick 1Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void app_allStates_runOnTick1Hz(struct StateMachine *state_machine);

/**
 * On-tick 100Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 * @return True if the next state is not the fault state, otherwise false
 */
bool app_allStates_runOnTick100Hz(struct StateMachine *state_machine);