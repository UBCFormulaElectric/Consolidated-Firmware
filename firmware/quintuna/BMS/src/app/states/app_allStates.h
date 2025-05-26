#pragma once

/**
 * On-tick 1Hz function for every state in the given state machine
 */
void app_allStates_runOnTick1Hz(void);

/**
 * On-tick 100Hz function for every state in the given state machine
 * @return True if the next state is not the fault state, otherwise false
 */
bool app_allStates_runOnTick100Hz(void);