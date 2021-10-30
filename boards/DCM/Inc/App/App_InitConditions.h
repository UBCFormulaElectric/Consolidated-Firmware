#pragma once

struct InitConditions;

/**
 * Allocate and initialize the init state conditions
 * @return The create init state conditions whose ownership is given to the caller.
 */
struct InitConditions *App_InitConditions_Create();

/**
 * Set the initial position of the start switch. This should only be called on entry to the init state.
 * @param init_conditions The given init state conditions to set the initial switch position on
 * @param position The initial position of the start switch
 */
void App_InitConditions_SetInitialSwitchPosition(struct InitConditions *init_conditions, bool position);

/**
 * Mark the start switch as having been toggled based on whether it was in the down position
 * @param init_conditions The given init state conditions to set whether the switch toggled on
 * @param start_switch The current state of the start switch
 */
void App_InitConditions_SetSwitchToggled(struct InitConditions *init_conditions, bool start_switch);

/**
 * Check whether we are allowed to transition from the init state
 * @param init_conditions The given init state conditions to check against
 * @param start_switch The current state of the start switch
 * @param break_actuated Whether the break is currently actuated
 * @return Whether we are allowed to transition
 */
bool App_InitConditions_CanTransition(struct InitConditions *init_conditions, bool start_switch, bool break_actuated);

/**
 * Deallocate the memory used by the given init state conditions
 * @param init_conditions The given init state conditions to deallocate
 */
void App_InitConditions_Destroy(struct InitConditions *init_conditions);
