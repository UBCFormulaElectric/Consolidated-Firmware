#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Change the selected Mux Channel
 * @param channel to change to
 */
void Io_ThermistorReadings_MuxSelect(uint8_t channel);

/**
 * Read the temperature of the current channel
 * @return Thermistor temperature in degrees C
 */
float Io_ThermistorReadings_ReadSelectedTemp(void);
