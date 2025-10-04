#pragma once

#include <stdbool.h>

/**
 * Initializes some apps-related values
 */
void io_apps_init(void);

/**
 * Calculates pedal percentage by comparing the current angle to the max angle
 * @return pedal percentage based on primary accelerator pedal sensor
 */
float io_apps_getPrimary(void);

/**
 * @return whether the primary accelerator pedal sensor is open or short circuit
 */
bool io_apps_isPrimaryOCSC(void);

/**
 * Calculates pedal percentage by comparing the current angle to the max angle
 * @return pedal percentage based on secondary accelerator pedal sensor
 */
float io_apps_getSecondary(void);

/**
 * @return whether the secondary accelerator pedal sensor is open or short circuit
 */
bool io_apps_isSecondaryOCSC(void);
