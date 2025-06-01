#pragma once

#include "app_utils.h"
#include <stdint.h>
#include <stdbool.h>

#define NUM_AUX_THERMISTORS 8

/**
 * Change the selected Mux Channel
 * @param channel to change to
 */
void io_thermistors_muxSelect(uint8_t channel);

/**
 * Read the temperature of the current channel
 * @param raw_voltage from
 * @return Thermistor temperature in degrees C
 */
float io_thermistors_readSelectedTemp(void);
