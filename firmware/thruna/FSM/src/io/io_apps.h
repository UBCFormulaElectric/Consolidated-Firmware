#pragma once

#include <stdbool.h>

/**
 * @return Percentage of press of accelerator pedal
 */
float io_apps_getPrimary(void);

/**
 * @return whether or not the primary accelerator pedal sensor is open or short circuit
 */
bool io_apps_isPrimaryOCSC(void);

/**
 * Gets the percentage of the accelerator pedal
 * @return The reported percentage travel of the secondary accelerator pedal sensor
 */
float io_apps_getSecondary(void);

/**
 * @return whether or not the secondary accelerator pedal sensor is open or short circuit
 */
bool io_apps_isSecondaryOCSC(void);
