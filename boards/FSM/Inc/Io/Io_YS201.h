#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initializes the primary and secondary YS-201 flow meters
 * @param htim The timer handle for both flow meters
 */
void Io_YS201_Init(TIM_HandleTypeDef *htim);

/**
 * Get flow rate from the primary YS-201 flow meter
 * @return The flow rate (L/min) from the primary flow meter
 */
float Io_YS201_GetPrimaryFlowRate(void);

/**
 * Get flow rate (L/min) from the YS-201 secondary flow meter
 * @return The flow rate (L/min) from the secondary flow meter
 */
float Io_YS201_GetSecondaryFlowRate(void);

/**
 * Update the frequency for the primary and secondary YS-201 flow meter's PWM
 * outputs
 * @note This function should be called in the input capture callback function
 * @param htim The handle of the timer that triggered the input capture
 *              callback function
 */
void Io_YS201_InputCaptureCallback(TIM_HandleTypeDef *const htim);

/**
 * Check if the PWM signal of the primary YS-201 flow meter is active. If it is
 * inactive (i.e. It has been unplugged or unpowered), set the frequency to NaN
 *
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal of the primary flow meter
 */
void Io_YS201_CheckIfPrimaryIsActive(void);

/**
 * Check if the PWM signal of the secondary YS-201 flow meter is active. If it
 * is inactive (i.e. It has been unplugged or unpowered), set the frequency to
 * NaN
 *
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal of the primary flow meter
 */
void Io_YS201_CheckIfSecondaryIsActive(void);
