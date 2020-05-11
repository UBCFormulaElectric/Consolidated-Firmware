#pragma once

#include "App_SharedStateMachine.h"

/**
 * On-tick function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_AllStatesRunOnTick(struct StateMachine *state_machine);
