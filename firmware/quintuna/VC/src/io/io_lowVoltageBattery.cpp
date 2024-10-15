#include "io_lowVoltageBattery.h"

// Percent error used to compensate for resistor errors. Determined from
// testing with the HW
#define VBAT_R_ERROR_COMPENSATION (1.044f)
#define VBOOST_R_ERROR_COMPENSATION (1.0f)
#define VACC_R_ERROR_COMPENSATION (1.0f)

// Voltage divider for voltage sensing
#define VBAT_VOLTAGE_DIV (220e3f / (220e3f + 330e3f))
#define VBOOST_VOLTAGE_DIV (3.1e3f / (20e3f + 3.1e3f))
#define VACC_VOLTAGE_DIV (3.1e3f / (20e3f + 3.1e3f))

static const LvBatteryConfig *config = NULL;

void io_lowVoltageBattery_init(const LvBatteryConfig *lv_batt_config)
{
    config = lv_batt_config;
}

bool io_lowVoltageBattery_hasChargeFault(void)
{
    // FAULT pin on LT3650 2-cell Li-Ion battery charger is pulled low when the chip detects a fault.
    return !hw_gpio_readPin(&config->lt3650_charger_fault_gpio);
}

bool io_lowVoltageBattery_hasBoostControllerFault(void)
{
    // PGOOD pin on LTC3786 boost converter is pulled low when the output voltage is more than ±10 % away from the
    // regulated output voltage.
    return !hw_gpio_readPin(&config->ltc3786_boost_fault_gpio);
}

float io_lowVoltageBattery_getBatVoltage(void)
{
    float adc_reading = hw_adc_getVoltage(config->vbat_vsense_adc_channel);
    return adc_reading * VBAT_R_ERROR_COMPENSATION / VBAT_VOLTAGE_DIV;
}

float io_lowVoltageBattery_getBoostVoltage(void)
{
    float adc_reading = hw_adc_getVoltage(config->boost_vsense_adc_channel);
    return adc_reading * VBOOST_R_ERROR_COMPENSATION / VBOOST_VOLTAGE_DIV;
}

float io_lowVoltageBattery_getAccVoltage(void)
{
    float adc_reading = hw_adc_getVoltage(config->acc_vsense_adc_channel);
    return adc_reading * VACC_R_ERROR_COMPENSATION / VACC_VOLTAGE_DIV;
}