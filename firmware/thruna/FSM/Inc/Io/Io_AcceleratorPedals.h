#pragma once

#include <stdbool.h>

/**
 * Calculates pedal percentage by comparing the current angle to the max angle relative to the rest angle
 * @return pedal percentage based on primary accelerator pedal sensor
 */
float Io_AcceleratorPedals_GetPapps(void);
/**
 * @return whether or not the primary accelerator pedal sensor is open or short circuit
 */
bool Io_AcceleratorPedals_PappsOCSC(void);

/**
 * Calculates pedal percentage by comparing the current angle to the max angle relative to the rest angle
 * @return pedal percentage based on secondary accelerator pedal sensor
 */
float Io_AcceleratorPedals_GetSapps(void);

/**
 * @return whether or not the secondary accelerator pedal sensor is open or short circuit
 */
bool Io_AcceleratorPedals_SappsOCSC(void);
