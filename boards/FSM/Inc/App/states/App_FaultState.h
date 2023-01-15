#pragma once

#include "App_SharedStateMachine.h"

void App_FaultStateRunOnEntry(struct StateMachine *const state_machine);

/**
 * On-tick 1Hz function for Fault state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_FaultStateRunOnTick1Hz(struct StateMachine *state_machine);

/**
 * On-tick 100Hz function for Fault state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_FaultStateRunOnTick100Hz(struct StateMachine *state_machine);

/**
 * Exit function for Fault State
 * @param state_machine
 */
void App_FaultStateRunOnExit(struct StateMachine *const state_machine);

/**
 * Get a pointer to the Fault STATE
 * @return A pointer to the Fault STATE
 */
const struct State *App_GetFaultState(void);
