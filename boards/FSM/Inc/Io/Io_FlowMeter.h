#pragma once

#include <stm32f3xx_hal.h>

/**
 * Get flow rate (L/min) from the primary flow meter
 */
float Io_FlowMeter_GetPrimaryFlowRate(void);

/**
 * Updates the frequency for the flow meter's PWM output. This
 * should be called in the input capture callback function.
 * @param htim The handle of the timer that triggered the input capture callback
 * function
 */
void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim);
