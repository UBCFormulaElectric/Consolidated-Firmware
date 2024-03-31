#pragma once

#include <stdbool.h>
#include <stdint.h>
/**
 * Initializes the i2c interface with the potentiometer.
 * @return True if i2c potentiometer is ready, else false.
 */
bool io_i2cPotentiometer_Init();

/**
 * Sets potentiometer wiper.
 * @param position Wiper position, 0 (min) to 127 (full).
 */
void io_i2cPotentiometer_setWiper(uint8_t position);

/**
 * Reads the current state of the wiper.
 * @return Current state of the wiper, 0 to 127.
 */
uint8_t io_i2cPotentiometer_readWiper();