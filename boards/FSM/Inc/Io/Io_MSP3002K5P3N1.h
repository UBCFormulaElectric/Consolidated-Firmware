#pragma once

#include <stdbool.h>

/**
 * Check if the MSP-300-2K5-P-3-N-1 is in an open circuit or short circuit
 * state.
 * @return True if the sensor is open or short circuited, false if the sensor is
 * not
 */
bool Io_MSP3002K5P3N1_IsOpenOrShortCircuit(void);

/**
 * Get the pressure in psi from the MSP-300-2K5-P-3-N-1 brake pressure sensor
 * @return The pressure of the brake pressure sensor in psi
 */
float Io_MSP3002K5P3N1_GetPressurePsi(void);
