#pragma once

#include "App_SharedStateMachine.h"
#include "App_CurrentLimit.h"

/**
 * Initialize state machine all states module.
 */
void App_AllStates_Init(void);

/**
 * On-tick 1Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_AllStatesRunOnTick1Hz(struct StateMachine *state_machine);

/**
 * On-tick 100Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 * @return True if the next state is not the fault state, otherwise false
 */
bool App_AllStatesRunOnTick100Hz(struct StateMachine *state_machine);

/**
 * On-tick 1kHz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_AllStatesRunOnTick1kHz(struct StateMachine *state_machine);