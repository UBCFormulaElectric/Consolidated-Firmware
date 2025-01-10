#include "io_lowVoltageBattery.h"
#include "hw_gpios.h"
#include "hw_adcs.h"

// Percent error used to compensate for resistor errors. Determined from
// testing with the HW
#define VBAT_R_ERROR_COMPENSATION (1.044f)
#define VBOOST_R_ERROR_COMPENSATION (1.0f)
#define VACC_R_ERROR_COMPENSATION (1.0f)

// Voltage divider for voltage sensing
#define VBAT_VOLTAGE_DIV (220e3f / (220e3f + 330e3f))
#define VBOOST_VOLTAGE_DIV (3.1e3f / (20e3f + 3.1e3f))
#define VACC_VOLTAGE_DIV (3.1e3f / (20e3f + 3.1e3f))

bool io_lowVoltageBattery_hasChargeFault(void) {
    // FAULT pin on LT3650 2-cell Li-Ion battery charger is pulled low when the chip detects a fault.
    return !hw_gpio_readPin(&nchrg_fault);
}

bool io_lowVoltageBattery_hasBoostControllerFault(void) {
    // PGOOD pin on LTC3786 boost converter is pulled low when the output voltage is more than ±10 % away from the
    // regulated output voltage.
    return !hw_gpio_readPin(&pgood);
}

float io_lowVoltageBattery_getBatVoltage(void) {
    return hw_adc_getVoltage(&vbat_sns) * VBAT_R_ERROR_COMPENSATION / VBAT_VOLTAGE_DIV;
}

float io_lowVoltageBattery_getBoostVoltage(void) {
    return hw_adc_getVoltage(&boost_22v_sns) * VBOOST_R_ERROR_COMPENSATION / VBOOST_VOLTAGE_DIV;
}

float io_lowVoltageBattery_getAccVoltage(void) {
    return hw_adc_getVoltage(&acc_24v_sns) * VACC_R_ERROR_COMPENSATION / VACC_VOLTAGE_DIV;
}