#pragma once

#include <stdbool.h>

#include "Io_Hal.h"

/**
 * Initializes a given timer handle for a flow meter
 * @param htim : The timer handle for both flow meters
 */
void Io_FlowMeter_Init(TIM_HandleTypeDef *htim);
/**
 * @returns the flow rate (L/min) from the primary flow meter
 */
float Io_FlowMeter_GetFlowRate(void);
/**
 * Update the frequency for the primary flow meter's PWM outputs.
 * @note This function should be called in the input capture callback function.
 * @param htim: The handle of the timer that triggered the input capture
 *              callback function
 */
void Io_FlowMeter_InputCaptureCallback(TIM_HandleTypeDef *htim);
/**
 * Check if the PWM signal of the primary flow meter is active. If it is
 * inactive (i.e. It has been unplugged or unpowered), set the frequency to NaN.
 *
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal of the primary flow meter.
 */
void Io_FlowMeter_CheckIfFlowMeterIsActive(void);

/**
 * @returns the temperature of the coolant at [sensor B] in Celsius
 */
float Io_Coolant_GetTemperatureA(void);
/**
 * @returns the temperature of the coolant at [sensor B] in Celsius
 */
float Io_Coolant_GetTemperatureB(void);
/**
 * @returns the pressure of the coolant at [sensor A] in PSI
 */
float Io_Coolant_GetPressureA(void);
/**
 * @returns the pressure of the coolant at [sensor B] in PSI
 */
float Io_Coolant_GetPressureB(void);
