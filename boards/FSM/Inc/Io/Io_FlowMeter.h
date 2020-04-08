#pragma once

#include <stm32f3xx_hal.h>

/**
 * Initializes the primary and secondary flow meter
 */
void Io_FlowMeter_Init(void);

/**
 * Get flow rate (L/min) from the primary flow meter
 */
float Io_FlowMeter_GetPrimaryFlowRate(void);

/**
 * Get flow rate (L/min) from the secondary flow meter
 */
float Io_FlowMeter_GetSecondaryFlowRate(void);

/**
 * Updates the frequency for the flow meter's PWM output. This
 * should be called in the input capture callback function.
 * @param htim The handle of the timer that triggered the input capture callback
 * function
 */
void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim);

/**
 * Checks to see if the timer CNT register has overflowed (see whether TIM
 * period has elapsed). Checks if positive edges are consistently detected from
 * the flow meter and displays a flow meter frequency of 0.0 Hz in situations
 * where there are no edge triggered interrupts (duty cycle is 0% or 100%)
 */
void Io_FlowMeter_Timer_Period_Elapsed_Tick(void);
