#pragma once

#include <stdbool.h>

namespace io::apps
{
/**
 * Initializes some apps-related values
 */
void init(void);

/**
 * Calculates pedal percentage by comparing the current angle to the max angle
 * @return pedal percentage based on primary accelerator pedal sensor
 */
float getPrimary(void);

/**
 * @return whether the primary accelerator pedal sensor is open or short circuit
 */
bool isPrimaryOCSC(void);

/**
 * Calculates pedal percentage by comparing the current angle to the max angle
 * @return pedal percentage based on secondary accelerator pedal sensor
 */
float getSecondary(void);

/**
 * @return whether the secondary accelerator pedal sensor is open or short circuit
 */
bool isSecondaryOCSC(void);
} // namespace io::apps
