#pragma once

#include <stdbool.h>
#include "app_utils.h"

class LowVoltageBattery
{
    /**
     * Check if the charging IC for the given low voltage battery has a fault.
     * @return true if the charging IC for the given low voltage battery has a
     *         fault, else false.
     */
    bool hasChargeFault(void);

    /**
     * Check if the boost controller IC for the given low voltage battery has a
     * fault.
     * @return true if the boost controller IC for the given low voltage battery
     *         has a fault, else false.
     */
    bool hasBoostControllerFault(void);

    /**
     * Get the 2S1P 18650 battery voltage.
     * @param low_voltage_battery the low voltage battery to read
     * @return 18650 series voltage, in volts.
     */
    float getBatVoltage(void);

    /**
     * Get the boost converter voltage.
     * @param low_voltage_battery the low voltage battery to get boost converter voltage for
     * @return Boost converter voltage, in volts.
     */
    float getBoostVoltage(void);

    /**
     * Get the accumulator LV voltage.
     * @param low_voltage_battery the low voltage battery to get accumulator voltage for
     * @return LV accumulator voltage, in volts.
     */
    float getAccVoltage(void);
};
