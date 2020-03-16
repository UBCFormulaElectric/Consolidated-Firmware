#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initialize Flowmeter
 */
void Io_FlowMeter_Init(void);

/**
 * Get Flow Rate (L/min) from the Flow Meter
 */
float Io_FlowMeter_GetFlowRate(void);

/**
 * Called in input capture callback fxn
 * @param htim
 */
void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim);
