#pragma once

#include "App_SharedStateMachine.h"

void AllStatesRunOnEntry(struct StateMachine *state_machine);
void AllStatesRunOnTick(struct StateMachine *state_machine);
void AllStatesRunOnExit(struct StateMachine *state_machine);
