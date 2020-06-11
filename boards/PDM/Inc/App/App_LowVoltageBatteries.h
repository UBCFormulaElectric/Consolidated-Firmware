#pragma once

#include <stdbool.h>

/**
 * Allocate and initialize low voltage batteries
 * @note The low voltage batteries are modeled after the 18650s on our vehicle,
 *       which are connected to:
 *         - A cell balancing IC
 *         - A charging IC
 *         - A boost controller IC
 * @param is_overvoltage A function that can be called to check if the cell
 *                       balancing IC for the low voltage batteries senses an
 *                       overvoltage
 * @param has_charge_fault A function that can be called to check if the cell
 *                         balanacing IC for the low voltage batteries has a
 *                         fault
 * @param has_boost_fault A function that can be called to check if the boost
 *                        controller IC for the low voltage batterise has a
 *                        fault
 * @return The created low voltage batteries, whose ownership is given to the
 *         caller
 */
struct LowVoltageBatteries *App_LowVoltageBatteries_Create(
    bool (*is_overvoltage)(void),
    bool (*has_charge_fault)(void),
    bool (*has_boost_fault)(void));

/**
 * Deallocate the memory used by the given low voltage batteries
 * @param low_voltage_batteries The low voltage batteries to deallocate
 */
void App_LowVoltageBatteries_Destroy(
    struct LowVoltageBatteries *low_voltage_batteries);

/**
 * Check if the cell balancing IC for the given low voltage batteries senses an
 * overvoltage
 * @return true if the cell balancing IC for the given low voltage batteries
 *         sense an overvoltage, else false
 */
bool App_LowVoltageBatteries_IsOvervoltage(
    const struct LowVoltageBatteries *low_voltage_batteries);

/**
 * Check if the charging IC for the given low voltage batteries has a fault
 * @return true if the charging IC for the given low voltage batteries has a
 *         fault, else false
 */
bool App_LowVoltageBatteries_HasChargeFault(
    const struct LowVoltageBatteries *low_voltage_batteries);

/**
 * Check if the boost controller IC for the given low voltage batteries has a
 * fault
 * @return true if the boost controller IC for the given low voltage batteries
 *         has a fault, else false
 */
bool App_LowVoltageBatteries_HasBoostControllerFault(
    const struct LowVoltageBatteries *low_voltage_batteries);
