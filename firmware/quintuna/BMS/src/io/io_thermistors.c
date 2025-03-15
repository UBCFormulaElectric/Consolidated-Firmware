#include "io_thermistors.h"
#include "hw_gpios.h"
#include "hw_adcs.h"

#define BIAS_RESISTOR_OHM (10000.0f)
#define REFERENCE_VOLTAGE (3.3f) // should this be 4.2V ?
#define THERM_LOOKUP_STARTING_TEMP (255.0f)
#define THERM_INDEX_TO_DEGC (0.5f)

void io_thermistors_muxSelect(const uint8_t channel)
{
    hw_gpio_writePin(&tsense_sel0_pin, channel & 1 << 0);
    hw_gpio_writePin(&tsense_sel1_pin, channel & 1 << 1);
    hw_gpio_writePin(&tsense_sel2_pin, channel & 1 << 2);
}

float io_thermistors_readSelectedTemp(void)
{
    const float raw_voltage = hw_adc_getVoltage(&aux_tsns);
}

float io_thermistors_voltageToTemperature(float raw_voltage, float temp_resistance_lut[])
{
    const uint8_t size_of_lut = sizeof(temp_resistance_lut) / sizeof(temp_resistance_lut[0]);
    const float thermistor_resistance = (raw_voltage * BIAS_RESISTOR_OHM) / (REFERENCE_VOLTAGE - raw_voltage);

    float thermistor_temp = THERM_LOOKUP_STARTING_TEMP;

    if ((thermistor_resistance <= temp_resistance_lut[0]) &&
        (thermistor_resistance >= temp_resistance_lut[size_of_lut - 1U]))
    {
        uint8_t therm_lut_index = 0U;
        for (therm_lut_index = 0U; thermistor_resistance < temp_resistance_lut[therm_lut_index]; therm_lut_index++);

        thermistor_temp = therm_lut_index * THERM_INDEX_TO_DEGC;
    }

    return thermistor_temp;
}
