#pragma once

#include <stdint.h>

namespace io::imd
{
/**
 * Get the frequency of the IMD's PWM output
 * @return The frequency of the IMD's PWM output
 */
float getFrequency(void);

/**
 * Get the duty cycle of the IMD's PWM output
 * @return The duty cycle of the IMD's PWM output
 */
float getDutyCycle(void);

/**
 * Get the time elapsed since the IMD was powered on
 * @return The the time elapsed since the IMD was powered on, in seconds
 */
uint32_t getTimeSincePowerOn(void);

/**
 * Increments software counter that checks how long ago IMD pwm signal was received
 * @return The current count of the counter
 */
uint8_t pwmCounterTick(void);
} // namespace io::imd