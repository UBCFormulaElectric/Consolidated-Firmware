#pragma once

#include "App_SharedStateMachine.h"

void App_DriveStateRunOnEntry(struct StateMachine *const state_machine);

/**
 * On-tick 1Hz function for drive state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_DriveStateRunOnTick1Hz(struct StateMachine *state_machine);

/**
 * On-tick 100Hz function for drive state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_DriveStateRunOnTick100Hz(struct StateMachine *state_machine);

/**
 *  Exit function for Drive State
 * @param state_machine
 */
void App_DriveStateRunOnExit(struct StateMachine *const state_machine);

/**
 * Get a pointer to the Drive STATE
 * @return A pointer to the Drive STATE
 */
const struct State *App_GetDriveState(void);
