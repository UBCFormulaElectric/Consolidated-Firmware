#pragma once

#include "hw_gpio.h"

/**
 * Initialize the LV battery monitor.
 */
bool io_lowVoltageBattery_init();

/**
 * Gets current from low voltage battery ()
 * @return current from battery
 */
float io_lowVoltageBattery_getPackVoltage();
