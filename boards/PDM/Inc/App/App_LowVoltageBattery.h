#pragma once

#include <stdbool.h>

/**
 * Allocate and initialize low voltage battery
 * @note The low voltage battery are modeled after the 18650s on our vehicle,
 *       which are connected to:
 *         - A charging IC
 *         - A boost controller IC
 * @param has_charge_fault A function that can be called to check if the cell
 *                         balanacing IC for the low voltage battery has a
 *                         fault
 * @param has_boost_fault A function that can be called to check if the boost
 *                        controller IC for the low voltage batterise has a
 *                        fault
 * @return The created low voltage battery, whose ownership is given to the
 *         caller
 */
struct LowVoltageBattery *App_LowVoltageBattery_Create(bool (*has_charge_fault)(void), bool (*has_boost_fault)(void));

/**
 * Deallocate the memory used by the given low voltage battery
 * @param low_voltage_battery The low voltage battery to deallocate
 */
void App_LowVoltageBattery_Destroy(struct LowVoltageBattery *low_voltage_battery);

/**
 * Check if the charging IC for the given low voltage battery has a fault
 * @return true if the charging IC for the given low voltage battery has a
 *         fault, else false
 */
bool App_LowVoltageBattery_HasChargeFault(const struct LowVoltageBattery *low_voltage_battery);

/**
 * Check if the boost controller IC for the given low voltage battery has a
 * fault
 * @return true if the boost controller IC for the given low voltage battery
 *         has a fault, else false
 */
bool App_LowVoltageBattery_HasBoostControllerFault(const struct LowVoltageBattery *low_voltage_battery);
