#include "io_thermistors.h"
#include "hw_gpios.h"
#include "hw_adcs.h"

#define SIZE_OF_TEMPERATURE_LUT (21U)
#define BIAS_RESISTOR_OHM (10000.0f)
#define REFERENCE_VOLTAGE (3.3f) // should this be 4.2V ?
#define THERM_INDEX_TO_DEGC (0.5f)
#define THERM_LOOKUP_STARTING_TEMP (255.0f)

#define LUT_RESOLUTION (5U)

// A 0-100°C temperature reverse lookup table with 5°C resolution for a
// B57861S0103A039 thermistor. The 0th index represents 0°C. Incrementing the
// index represents a 5°C increase in temperature.
static const float temp_resistance_lut[SIZE_OF_TEMPERATURE_LUT] = {
    32650.0f, 25390.0f, 19900.0f, 15710.0f, 12490.0f, 10000.0f, 8057.0f, 6531.0f, 5327.0f, 4369.0f, 3603.0f,
    2986.0f,  2488.0f,  2083.0f,  1752.0f,  1481.0f,  1258.0f,  1072.0f, 917.7f,  788.5f,  680.0f
};

void io_thermistors_muxSelect(const uint8_t channel)
{
    hw_gpio_writePin(&tsense_sel0_pin, channel & 1 << 0);
    hw_gpio_writePin(&tsense_sel1_pin, channel & 1 << 1);
    hw_gpio_writePin(&tsense_sel2_pin, channel & 1 << 2);
}

float io_thermistors_readSelectedTemp(void)
{
    const float raw_voltage = hw_adc_getVoltage(&aux_tsns);

    float thermistor_temp = THERM_LOOKUP_STARTING_TEMP;
    const float thermistor_resistance = (raw_voltage * BIAS_RESISTOR_OHM) / (REFERENCE_VOLTAGE - raw_voltage);

    // Check that the thermistor resistance is in range
    if ((thermistor_resistance <= temp_resistance_lut[0]) &&
        (thermistor_resistance >= temp_resistance_lut[SIZE_OF_TEMPERATURE_LUT - 1U]))
    {
        // Find the indexes of the two resistances that the calculated resistance lies between
        uint8_t therm_lut_index = 0U;
        for (therm_lut_index = 0U; thermistor_resistance < temp_resistance_lut[therm_lut_index]; therm_lut_index++)
            ;
        
        if (therm_lut_index == 0U)
        {
            thermistor_temp = temp_resistance_lut[therm_lut_index];
        } else
        {
            // Linear interpolation to find thermistor temperature
            uint8_t y2 = temp_resistance_lut[therm_lut_index];
            uint8_t y1 = temp_resistance_lut[therm_lut_index - 1];
            uint8_t x2 = therm_lut_index * LUT_RESOLUTION;
            uint8_t x1 = (therm_lut_index - 1) * LUT_RESOLUTION;
        
            thermistor_temp = (thermistor_resistance - y1) * ((x2 - x1) / (y2 - y1)) + x1;
        }
    }

    return thermistor_temp;
}
