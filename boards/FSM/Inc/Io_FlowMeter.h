#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initialize Flowmeter
 */
void Io_FlowMeter_Init(void);

/**
 * Get frequency from PWM input
 * @return
 */
float Io_Imd_GetFrequency(void);

/**
 * Called in input capture callback fxn
 * @param htim
 */
void Io_flowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim);
