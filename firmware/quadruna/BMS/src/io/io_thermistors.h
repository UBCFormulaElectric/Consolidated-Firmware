#pragma once

#include <stdint.h>

/**
 * Change the selected Mux Channel
 * @param channel to change to
 */
void io_thermistors_muxSelect(uint8_t channel);

/**
 * Read the temperature of the current channel
 * @return Thermistor temperature in degrees C
 */
float io_thermistors_readSelectedTemp(void);
