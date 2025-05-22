#include "app_thermistor.h"
#include <assert.h>
#include <stddef.h>

float app_thermistor_resistanceToTemp(const float thermistor_resistance, const ThermistorLUT *temp_resistance_lut)
{
    assert(temp_resistance_lut != NULL);
    // Check that the thermistor resistance is in range
    if (!(thermistor_resistance <= temp_resistance_lut->resistances[0] &&
          thermistor_resistance >= temp_resistance_lut->resistances[temp_resistance_lut->size - 1U]))
    {
        return -1.0f; // TODO maybe pick a better sentinel value
    }

    // Find the indexes of the two resistances that the calculated resistance lies between
    uint16_t low_index  = 0U;
    uint16_t high_index = temp_resistance_lut->size - (uint16_t)1;

    // bsearch
    while (low_index <= high_index)
    {
        const uint16_t mid_index = low_index + (uint16_t)((high_index - low_index) / 2);
        if (thermistor_resistance > temp_resistance_lut->resistances[mid_index])
        {
            high_index = mid_index - 1; // search lower half of LUT
        }
        else
        {
            low_index = mid_index + 1; // search upper half of LUT
        }
    }

    const uint16_t therm_lut_index = low_index;
    if (therm_lut_index == temp_resistance_lut->size - 1U)
    {
        return temp_resistance_lut->starting_temp +
               (float)(temp_resistance_lut->size - 1U) * temp_resistance_lut->resolution;
    }
    // Linear interpolation to find thermistor temperature
    const float y2 = temp_resistance_lut->resistances[therm_lut_index];
    const float y1 = temp_resistance_lut->resistances[therm_lut_index + 1];
    const float x2 = temp_resistance_lut->starting_temp + (float)therm_lut_index * temp_resistance_lut->resolution;
    const float x1 =
        temp_resistance_lut->starting_temp + (float)(therm_lut_index + 1) * temp_resistance_lut->resolution;

    return (thermistor_resistance - y1) * ((x2 - x1) / (y2 - y1)) + x1;
}