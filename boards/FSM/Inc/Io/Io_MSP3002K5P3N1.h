#pragma once

/**
 * Check if the pressure from MSP-300-2K5-P-3-N-1 exceeds the brake pressure
 * threshold. If the pressure exceeds threshold, the brake is said to be
 * actuated.
 * @return True if the brake is actuated, false if the brake is not actuated
 */
bool Io_MSP3002K5P3N1_BrakeIsActuated(void);

/**
 * Get the pressure in psi from the MSP-300-2K5-P-3-N-1 brake pressure sensor
 * @return The pressure of the brake pressure sensor in psi
 */
float Io_MSP3002K5P3N1_GetPressurePsi(void);
