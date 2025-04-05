#pragma once

#include <stdint.h>

typedef struct
{
    uint8_t starting_temp; // Temperature at 0th index (deg C)
    uint8_t resolution; // Degree celsius increase per index step
    uint16_t size; // Number of LUT entries
    float resistances[]; // Pointer to resistance values (Ohms)
} ThermistorLUT;

/**
 * Calculate temperature based on thermistor temperature and LUT
 * @param thermistor_resistance resistance of the thermistor
 * @param temp_resistance_lut reverse lookup table
 * @return Thermistor temperature in degrees C
 */
float io_thermistor_resistanceToTemp(float thermistor_resistance, ThermistorLUT temp_resistance_lut);