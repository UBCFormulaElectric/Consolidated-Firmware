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

bool hasChargeFault(void)
{
    // FAULT pin on LT3650 2-cell Li-Ion battery charger is pulled low when the chip detects a fault.
    return !hw::gpio::nchrg_fault.readPin();
}

bool hasBoostControllerFault(void)
{
    // PGOOD pin on LTC3786 boost converter is pulled low when the output voltage is more than ±10 % away from the
    // regulated output voltage.
    return !hw::gpio::pgood.readPin();
}

float getBatVoltage(void)
{
    float adc_reading = hw::adc::vbat_vsense.getVoltage();
    return adc_reading * VBAT_R_ERROR_COMPENSATION / VBAT_VOLTAGE_DIV;
}

float getBoostVoltage(void)
{
    float adc_reading = hw::adc::boost_vsense.getVoltage();
    return adc_reading * VBOOST_R_ERROR_COMPENSATION / VBOOST_VOLTAGE_DIV;
}

float getAccVoltage(void)
{
    float adc_reading = hw::adc::acc_vsense.getVoltage();
    return adc_reading * VACC_R_ERROR_COMPENSATION / VACC_VOLTAGE_DIV;
}
