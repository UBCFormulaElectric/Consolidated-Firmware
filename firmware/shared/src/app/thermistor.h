#pragma once

#include <stdint.h>

typedef struct
{
    float    starting_temp; // Temperature at 0th index (deg C)
    float    resolution;    // Degree celsius increase per index step
    uint16_t size;          // Number of LUT entries
    float   *resistances;   // Pointer to resistance values (Ohms)
} ThermistorLUT;

/**
 * Calculate temperature based on thermistor temperature and LUT
 * @param thermistor_resistance resistance of the thermistor
 * @param temp_resistance_lut reverse lookup table
 * @return Thermistor temperature in degrees C
 */
float app_thermistor_resistanceToTemp(float thermistor_resistance, const ThermistorLUT *temp_resistance_lut);