#pragma once

#include <stdbool.h>
#include "Io_Hal.h"

/**
 * Initialize the PWM input for measuring the IMD's PWM output
 */
void Io_Imd_Init(void);

/**
 * Get the frequency of the IMD's PWM output
 * @return The frequency of the IMD's PWM output
 */
float Io_Imd_GetFrequency(void);

/**
 * Get the duty cycle of the IMD's PWM output
 * @return The duty cycle of the IMD's PWM output
 */
float Io_Imd_GetDutyCycle(void);

/**
 * Updates the frequency and duty cycle for the IMD's PWM output. This should
 * be called in the input capture callback function.
 * @param htim The handle of the timer that triggered the input capture callback
 *             function
 */
void Io_Imd_InputCaptureCallback(TIM_HandleTypeDef *htim);

/**
 * Get the time elapsed since the IMD was powered on
 * @return The the time elapsed since the IMD was powered on, in seconds
 */
uint16_t Io_Imd_GetTimeSincePowerOn(void);

/**
 * Get the latched fault status of the IMD
 * @return The latched fault status of the IMD
 */
bool Io_Imd_CheckLatchedFault(void);

/**
 * Get the latched fault status of the BSPD
 * @return The latched fault status of the BSPD
 */
bool Io_Bspd_CheckLatchedFault(void);

/**
 * Get the latched fault status of the BMS
 * @return The latched fault status of the BMS
 */
bool Io_Bms_CheckLatchedFault(void);
