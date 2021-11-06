#pragma once

struct StartSwitch;

/**
 * Allocate and initialize the init state conditions
 * @return The create init state conditions whose ownership is given to the
 *         caller.
 */
struct StartSwitch *App_StartSwitch_Create();

/**
 * Set the initial position of the start switch. This should only be called on
 * entry to the init state.
 * @param start_switch The given init state conditions to set the initial switch
 *                     position on
 * @param position The initial position of the start switch
 */
void App_StartSwitch_SetInitialSwitchPosition(
    struct StartSwitch *start_switch,
    bool                position);

/**
 * Mark the start switch as having been toggled based on whether it was in the
 * down position
 * @param start_switch The given init state conditions to set whether the switch
 *                     toggled on
 * @param state The current state of the switch
 */
void App_StartSwitch_SetSwitchToggled(
    struct StartSwitch *start_switch,
    bool                state);

/**
 * Check whether the switch conditions have passed to transition from the init
 * state
 * @param start_switch The given init state conditions to check against
 * @param state The current state of the switch
 * @return Whether we are allowed to transition
 */
bool App_StartSwitch_CanTransition(
    struct StartSwitch *start_switch,
    bool                state);

/**
 * Deallocate the memory used by the given init state conditions
 * @param start_switch The given init state conditions to deallocate
 */
void App_StartSwitch_Destroy(struct StartSwitch *start_switch);
