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
struct LowVoltageBattery *App_LowVoltageBattery_Create(
    bool (*has_charge_fault)(void),
    bool (*has_boost_fault)(void),
    float (*get_bat_voltage)(void),
    float (*get_boost_voltage)(void),
    float (*get_aux_voltage)(void));

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

/**
 * Get the 18650 series cell voltage
 * @param low_voltage_battery the low voltage battery to read
 * @return 18650 series voltage
 */
float App_LowVoltageBattery_GetBatVoltage(struct LowVoltageBattery *low_voltage_battery);

/**
 * Get the Boost converter voltage
 * @param low_voltage_battery the low voltage battery to get boost converter voltage for
 * @return boost converter voltage
 */
float App_LowVoltageBattery_GetBoostVoltage(struct LowVoltageBattery *low_voltage_battery);

/**
 * Get the accumulator lv voltage
 * @param low_voltage_battery the low voltage battery to get accumulator voltage for
 * @return lv accumulator voltage
 */
float App_LowVoltageBattery_GetAccVoltage(struct LowVoltageBattery *low_voltage_battery);