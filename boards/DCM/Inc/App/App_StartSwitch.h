#pragma once

/**
 * StartSwitch is used to track the position of the start switch to determine
 * if it needs to be toggled off before we are allowed to transition into the
 * drive state. The switch needs to be toggled off when it starts in the on
 * position.
 */

struct StartSwitch;

/**
 * Allocate and initialize the start switch
 * @return The create start switch whose ownership is given to the caller.
 */
struct StartSwitch *App_StartSwitch_Create();

/**
 * Set the initial state of the start switch. This should only be called on
 * entry to the init state.
 * @param start_switch The given start switch to set the initial switch state on
 * @param state The initial state of the start switch
 */
void App_StartSwitch_SetInitialPosition(
    struct StartSwitch *start_switch,
    bool                state);

/**
 * Track the current position of the start switch and, if needed, mark it as
 * having been toggled off
 * @param start_switch The given start switch to set whether the switch toggled
 * on
 * @param state The current state of the switch
 */
void App_StartSwitch_SetPosition(struct StartSwitch *start_switch, bool state);

/**
 * Check whether we can transition from the init state to the drive state given
 * the start switch conditions
 * @param start_switch The given start switch to check against
 * @return Whether we are allowed to transition
 */
bool App_StartSwitch_CanTransitionToDriveState(
    struct StartSwitch *start_switch);

/**
 * Deallocate the memory used by the given start switch
 * @param start_switch The given start switch to deallocate
 */
void App_StartSwitch_Destroy(struct StartSwitch *start_switch);
