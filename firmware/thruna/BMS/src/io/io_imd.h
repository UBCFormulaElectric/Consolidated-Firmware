#pragma once

#include <stdint.h>

#ifdef TARGET_EMBEDDED
#include "hw_hal.h"
#endif

/**
 * Initialize the PWM input for measuring the IMD's PWM output
 */
void io_imd_init(void);

/**
 * Get the frequency of the IMD's PWM output
 * @return The frequency of the IMD's PWM output
 */
float io_imd_getFrequency(void);

/**
 * Get the duty cycle of the IMD's PWM output
 * @return The duty cycle of the IMD's PWM output
 */
float io_imd_getDutyCycle(void);

#ifdef TARGET_EMBEDDED
/**
 * Updates the frequency and duty cycle for the IMD's PWM output. This should
 * be called in the input capture callback function.
 * @param htim The handle of the timer that triggered the input capture callback
 *             function
 */
void io_imd_inputCaptureCallback(TIM_HandleTypeDef * htim);
#endif

/**
 * Get the time elapsed since the IMD was powered on
 * @return The the time elapsed since the IMD was powered on, in seconds
 */
uint16_t io_imd_getTimeSincePowerOn(void);