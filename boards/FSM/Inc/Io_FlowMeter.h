#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initialize the flow meter
 */
void Io_FlowMeter_Init(void);

/**
 * Get Flow Rate (L/min) from the flow meter
 */
float Io_FlowMeter_GetFlowRate(void);

/**
 * Called in input capture callback fxn
 * @param htim The handle of the timer that triggered the input capture
 * callback function
 */
void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim);
