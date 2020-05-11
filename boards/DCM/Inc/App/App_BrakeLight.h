#pragma once

#include <stdbool.h>

enum BrakeLight_Status
{
    BRAKE_LIGHT_ON,
    BRAKE_LIGHT_OFF,
};

/**
 * Allocate and initialize a brake light
 * @param is_brake_actuated A function that can be called to check if the brake
 *                          is actuated
 * @param is_regen_active A function that can be called to check if the regen
 *                        braking is active
 * @param turn_on_brake_light A function that can be called to turn on the brake
 *                            light
 * @param turn_off_brake_light A function that can be called to turn off the
 *                             brake light
 * @return A pointer to the created brake light, whose ownership is given to
 * the caller
 */
struct BrakeLight *App_BrakeLight_Create(
    bool (*const is_brake_actuated)(void),
    bool (*const is_regen_active)(void),
    void (*const turn_on_brake_light)(void),
    void (*const turn_off_brake_light)(void));

/**
 * Deallocate the memory used by the given brake light
 * @param brake_light The brake light to deallocate
 */
void App_BrakeLight_Destroy(struct BrakeLight *const brake_light);

/**
 * Update the status for the given brake light
 * @param brake_light The brake light to update for
 */
void App_BrakeLight_Tick(struct BrakeLight *const brake_light);

/**
 * Get the status for the given brake light
 * @param brake_light The brake light to get status for
 * @return The status for the given brake light
 */
enum BrakeLight_Status
    App_BrakeLight_GetStatus(const struct BrakeLight *const brake_light);
