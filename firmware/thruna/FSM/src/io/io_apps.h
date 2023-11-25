#pragma once

#include <stdbool.h>

/**
 * Performs initial calculations for the rest and max angle of PAPPS and SAPPS
 * @return
 */
void io_apps_init(void);

/**
 * Calculates pedal percentage by comparing the current angle to the max angle
 * @return pedal percentage based on primary accelerator pedal sensor
 */
float io_apps_getPrimary(void);

/**
 * @return whether or not the primary accelerator pedal sensor is open or short circuit
 */
bool io_apps_isPrimaryOCSC(void);

/**
 * Calculates pedal percentage by comparing the current angle to the max angle
 * @return pedal percentage based on secondary accelerator pedal sensor
 */
float io_apps_getSecondary(void);

/**
 * @return whether or not the secondary accelerator pedal sensor is open or short circuit
 */
bool io_apps_isSecondaryOCSC(void);
