#pragma once

#include <stdint.h>
#include <stdbool.h>
/**
 * Change the selected Mux Channel
 * @param channel to change to
 * @return true if valid channel value
 */
bool io_thermistors_muxSelect(uint8_t channel);

/**
 * Read the temperature of the current channel
 * @param raw_voltage from
 * @return Thermistor temperature in degrees C
 */
float io_thermistors_readSelectedTemp(void);
