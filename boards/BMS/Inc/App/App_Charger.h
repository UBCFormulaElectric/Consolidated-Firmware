#pragma once

#include <stdbool.h>

struct Charger;

/**
 * Allocate and initialize a charger
 * @param enable_charger A function that can be called to turn on the charger
 * @param disable_charger A function that can be called to turn off the charger
 * @param is_charger_connected A function that returns whether the charger is
 *                             is connected
 * @return The created charger, whose ownership is given to the caller
 */
struct Charger *App_Charger_Create(
    void (*enable_charger)(void),
    void (*disable_charger)(void),
    bool (*is_charger_connected)(void));

/**
 * Deallocate the memory used by the given charger
 * @param charger The charger to deallocate
 */
void App_Charger_Destroy(struct Charger *charger);

/**
 * Enable the given charger
 * @param charger The charger to enable
 */
void App_Charger_Enable(struct Charger *charger);

/**
 * Disable the given charger
 * @param charger The charger to disable
 */
void App_Charger_Disable(struct Charger *charger);

/**
 * Check if the given charger is connected
 * @param charger The charger to check
 * @return true if the charger is connected, else false
 */
bool App_Charger_IsConnected(struct Charger *charger);
