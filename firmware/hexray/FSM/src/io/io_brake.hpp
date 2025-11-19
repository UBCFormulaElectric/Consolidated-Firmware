#pragma once

#include <stdbool.h>

namespace io::brake
{
/**
 * Check if the brake is actuated, by getting the digital signal from the comparator on the FSM
 * @return True if the brake is actuated, false if the brake is not actuated
 */
bool isActuated(void);

/**
 * Get the pressure in psi from the MSP-300-2K5-P-3-N-1 brake pressure sensor
 * @return The pressure of the primary brake pressure sensor in psi
 */
float getFrontPressurePsi(void);

/**
 * @return the hardware overcurrent/short-circuit status for the brake system
 */
bool OCSC(void);
} // namespace io::brake
