#pragma once

#include "hw_hal.h"

/**
 * Initializes the i2c interface with the potentiometer.
 * @param i2c_handle I2c handler.
 * @return True if i2c potentiometer is ready, else false.
 */
bool io_i2c_potentiometer_init(I2C_HandleTypeDef i2c_handle);

/**
 * Sets potentiometer wiper.
 * @param value Wiper position, 0 (min) to 127 (full).
 */
void io_i2c_set_wiper(uint8_t position);
