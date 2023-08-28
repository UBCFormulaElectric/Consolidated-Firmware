#pragma once

#include <stdbool.h>

struct BinarySwitch;

/**
 * Allocate and initialize a binary switch
 * @param is_turned_on A function that returns a boolean indicating whether the
 *                     binary switch is turned on
 * @return The created binary switch, whose ownership is given to the caller
 */
struct BinarySwitch *App_BinarySwitch_Create(bool (*is_turned_on)(void));

/**
 * Deallocate the memory used by the given binary switch
 * @param binary_switch The binary switch to deallocate
 */
void App_BinarySwitch_Destroy(struct BinarySwitch *binary_switch);

/**
 * Check if the given binary switch is turned on
 * @param binary_switch The binary switch to check if it's turned on
 * @return true if the given binary switch is turned on, else false
 */
bool App_BinarySwitch_IsTurnedOn(const struct BinarySwitch *binary_switch);
