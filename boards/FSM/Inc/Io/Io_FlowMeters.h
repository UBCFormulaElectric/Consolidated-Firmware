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
float Io_FlowMeters_GetPrimaryFlowRate(void);

/**
 * Get flow rate (L/min) from the secondary flow meter
 */
float Io_FlowMeters_GetSecondaryFlowRate(void);

/**
 * Updates the frequency for the flow meter's PWM output. This
 * should be called in the input capture callback function.
 * @param htim : The handle of the timer that triggered the input capture
 * callback function
 */
void Io_FlowMeters_InputCaptureCallback(TIM_HandleTypeDef *htim);

/**
 * Detects when timer CNT register overflowed, and checks if positive edges are
 * detected from the primary flow meter. Outputs NaN in situations where edges
 * triggered interrupts are not detected.
 */
void Io_FlowMeters_Elapsed_TickPrimary(void);

/**
 * Detects when timer CNT register overflowed, and checks if positive edges are
 * detected from the secondary flow meter. Outputs NaN in situations where edges
 * triggered interrupts are not detected.
 */
void Io_FlowMeters_Elapsed_TickSecondary(void);

/**
 * Updates the measured frequency for the primary flow meter
 */
void Io_FlowMeters_TickPrimary(void);

/**
 * Updates the measured frequency for the secondary flow meter
 */
void Io_FlowMeters_TickSecondary(void);
