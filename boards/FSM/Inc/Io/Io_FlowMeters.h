#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initializes the primary and secondary flow meter
 * @param htim : The timer handle for both flow meters
 */
void Io_FlowMeters_Init(TIM_HandleTypeDef *htim);

/**
 * Get flow rate (L/min) from the primary flow meter
 */
float Io_FlowMeters_GetFlowRate(void);

/**
 * Update the frequency for the primary and secondary flow meter's PWM outputs.
 * @note This function should be called in the input capture callback function.
 * @param htim: The handle of the timer that triggered the input capture
 *              callback function
 */
void Io_FlowMeters_InputCaptureCallback(TIM_HandleTypeDef *htim);

/**
 * Check if the PWM signal of the primary flow meter is active. If it is
 * inactive (i.e. It has been unplugged or unpowered), set the frequency to NaN.
 *
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal of the primary flow meter.
 */
void Io_FlowMeters_CheckIfFlowMeterIsActive(void);
