#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initialize the flow meter
 */
void Io_FlowMeter_Init(void);

/**
 * Get flow rate (L/min) from the flow meter
 */
float Io_FlowMeter_GetFlowRate(void);

/**
 * Updates the frequency and duty cycle for the flower meter's PWM output. This should
 * be called in the input capture callback function.
 * @param htim The handle of the timer that triggered the input capture callback function
 */
void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim);
