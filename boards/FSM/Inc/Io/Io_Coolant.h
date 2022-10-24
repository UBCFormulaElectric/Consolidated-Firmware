#pragma once

#include <stm32f3xx_hal.h>

//==========FLOW METER==========
/**
 * Initializes the primary and secondary flow meter
 * @param htim : The timer handle for both flow meters
 */
void Io_FlowMeters_Init(TIM_HandleTypeDef *htim);

/**
 * @returns the flow rate (L/min) from the primary flow meter
 */
float Io_FlowMeters_GetFlowRate(void);

/**
 * Get flow rate (L/min) from the secondary flow meter
 */
float Io_FlowMeters_GetSecondaryFlowRate(void);

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

/**
 * Check if the PWM signal of the secondary flow meter is active. If it is
 * inactive (i.e. It has been unplugged or unpowered), set the frequency to NaN.
 *
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal of the primary flow meter.
 */
void Io_FlowMeters_CheckIfSecondaryIsActive(void);

//==========TEMPERATURE SENSORS==========
/**
 * @returns the temperature of the coolant at [sensor B] in Celsius
 */
float Io_GetTemperatureA(void);
/**
 * @returns the temperature of the coolant at [sensor B] in Celsius
 */
float Io_GetTemperatureB(void);

//==========PRESSURE SENSORS==========
/**
 * @returns the pressure of the coolant at [sensor A] in PSI
 */
float Io_GetPressureA(void);
/**
 * @returns the pressure of the coolant at [sensor B] in PSI
 */
float Io_GetPressureB(void);
