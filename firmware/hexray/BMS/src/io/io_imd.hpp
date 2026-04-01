#pragma once

#include <stdint.h>

namespace io::imd
{

/**
 * Get the frequency of the IMD's PWM output (returns 0.0f if no signal)
 */
float getFrequency(void);

/**
 * Get the duty cycle of the IMD's PWM output
 */
float getDutyCycle(void);

/**
 * Get the time elapsed since the IMD was powered on (in milliseconds)
 */
uint32_t getTimeSincePowerOn(void);
} // namespace io::imd