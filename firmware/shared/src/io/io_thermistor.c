#include "io_thermistor.h"
#include <assert.h>

float io_thermistor_resistanceToTemp(float thermistor_resistance, const ThermistorLUT* temp_resistance_lut)
{

    assert(temp_resistance_lut != NULL);
    float    thermistor_temp = -1.0;
    uint16_t max_lut_index   = temp_resistance_lut->size - 1U;

    // Check that the thermistor resistance is in range
    if ((thermistor_resistance <= temp_resistance_lut->resistances[0]) &&
        (thermistor_resistance >= temp_resistance_lut->resistances[max_lut_index]))
    {
        // Find the indexes of the two resistances that the calculated resistance lies between
        uint8_t low_index  = 0U;
        uint8_t high_index = max_lut_index;
        uint8_t mid_index  = (low_index + high_index) / 2;

        while (low_index <= high_index)
        {
            if (thermistor_resistance > temp_resistance_lut->resistances[mid_index])
            {
                high_index = mid_index - 1; // search lower half of LUT
            }
            else
            {
                low_index = mid_index + 1; // search upper half of LUT
            }

            mid_index = (low_index + high_index) / 2;
        }

        uint8_t therm_lut_index = mid_index;

        if (therm_lut_index == max_lut_index)
        {
            return temp_resistance_lut->starting_temp + (max_lut_index * temp_resistance_lut->resolution);
        }
        else
        {
            // Linear interpolation to find thermistor temperature
            float   y2 = temp_resistance_lut->resistances[therm_lut_index];
            float   y1 = temp_resistance_lut->resistances[therm_lut_index + 1];
            uint8_t x2 = temp_resistance_lut->starting_temp + (therm_lut_index * temp_resistance_lut->resolution);
            uint8_t x1 = temp_resistance_lut->starting_temp + ((therm_lut_index + 1) * temp_resistance_lut->resolution);

            thermistor_temp = (thermistor_resistance - y1) * ((x2 - x1) / (y2 - y1)) + x1;
        }
    }

    return thermistor_temp;
}