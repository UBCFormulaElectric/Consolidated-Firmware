#pragma once

#include "App_SharedMacros.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct
{
    const Gpio       lt3650_charger_fault_gpio;
    const Gpio       ltc3786_boost_fault_gpio;
    const AdcChannel vbat_vsense_adc_channel;
    const AdcChannel boost_vsense_adc_channel;
    const AdcChannel acc_vsense_adc_channel;
} LvBatteryConfig;
#else
EMPTY_STRUCT(LvBatteryConfig);
#endif

/**
 * Initialize the LV battery IO module.
 * @param config Config params.
 */
void io_lowVoltageBattery_init(const LvBatteryConfig * lv_batt_config);

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
 * @param low_voltage_battery the low voltage battery to read
 * @return 18650 series voltage, in volts.
 */
float io_lowVoltageBattery_getBatVoltage(void);

/**
 * Get the boost converter voltage.
 * @param low_voltage_battery the low voltage battery to get boost converter voltage for
 * @return Boost converter voltage, in volts.
 */
float io_lowVoltageBattery_getBoostVoltage(void);

/**
 * Get the accumulator LV voltage.
 * @param low_voltage_battery the low voltage battery to get accumulator voltage for
 * @return LV accumulator voltage, in volts.
 */
float io_lowVoltageBattery_getAccVoltage(void);