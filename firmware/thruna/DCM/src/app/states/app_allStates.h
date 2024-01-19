#pragma once

#include <stdbool.h>
#include "app_stateMachine.h"

/**
 * On-tick 100Hz function for every state in the given state machine
 * @return True if the next state is not the fault state, otherwise false
 */
bool app_allStates_runOnTick100Hz(void);
