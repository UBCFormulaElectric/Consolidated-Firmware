#pragma once

#include <stdbool.h>

/**
 * Initialize brake light module.
 */
void App_BrakeLight_Init();

/**
 * Turn on the given brake light if the brake is actuated and/or the regen is
 * active, else turn off the given brake light
 */
void App_BrakeLight_SetLightStatus(bool is_brake_actuated);

/**
 * Check if the given brake light is turned on
 * @return true if the brake light is turned on, else false
 */
bool App_BrakeLight_IsTurnedOn();
