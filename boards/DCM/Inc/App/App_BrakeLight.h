#pragma once

#include <stdbool.h>

/**
 * Allocate and initialize a brake light
 * @param turn_on_brake_light A function that can be called to turn on the brake
 *                            light
 * @param turn_off_brake_light A function that can be called to turn off the
 *                             brake light
 * @return The created brake light, whose ownership is given to the caller
 */
struct BrakeLight *App_BrakeLight_Create(void (*turn_on_brake_light)(void), void (*turn_off_brake_light)(void));

/**
 * Deallocate the memory used by the given brake light
 * @param brake_light The brake light to deallocate
 */
void App_BrakeLight_Destroy(struct BrakeLight *brake_light);

/**
 * Turn on the given brake light if the brake is actuated and/or the regen is
 * active, else turn off the given brake light
 * @param brake_light The brake light to turn on/off
 * @param is_brake_actuated True if the brake exceeds its actuation threshold,
 *                          else false
 * @param is_regen_active True if negative torque (regenetrative braking) is
 *                        being sent to the inverter, else false
 */
void App_BrakeLight_SetLightStatus(struct BrakeLight *brake_light, bool is_brake_actuated, bool is_regen_active);

/**
 * Check if the given brake light is turned on
 * @param brake_light The brake light to check if it's turned on
 * @return true if the brake light is turned on, else false
 */
bool App_BrakeLight_IsTurnedOn(const struct BrakeLight *brake_light);
