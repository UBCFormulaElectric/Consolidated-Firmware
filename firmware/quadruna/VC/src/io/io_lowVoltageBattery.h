#pragma once

#include <stdbool.h>

/**
 * Check if the charging IC for the given low voltage battery has a fault.
 * @return true if the charging IC for the given low voltage battery has a
 *         fault, else false.
 */
bool io_lowVoltageBattery_hasChargeFault(void);

/**
 * Check if the boost controller IC for the given low voltage battery has a
 * fault.
 * @return true if the boost controller IC for the given low voltage battery
 *         has a fault, else false.
 */
bool io_lowVoltageBattery_hasBoostControllerFault(void);

/**
 * Get the 2S1P 18650 battery voltage.
 * @return 18650 series voltage, in volts.
 */
float io_lowVoltageBattery_getBatVoltage(void);

/**
 * Get the boost converter voltage.
 * @return Boost converter voltage, in volts.
 */
float io_lowVoltageBattery_getBoostVoltage(void);

/**
 * Get the accumulator LV voltage.
 * @return LV accumulator voltage, in volts.
 */
float io_lowVoltageBattery_getAccVoltage(void);