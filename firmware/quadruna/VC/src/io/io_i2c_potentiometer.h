#pragma once

#include "hw_hal.h"
#include <stdbool.h>
/**
 * Initializes the i2c interface with the potentiometer.
 * @return True if i2c potentiometer is ready, else false.
 */
bool io_i2c_potentiometerInit();

/**
 * Sets potentiometer wiper.
 * @param position Wiper position, 0 (min) to 127 (full).
 */
void io_i2c_setWiper(uint8_t position);
