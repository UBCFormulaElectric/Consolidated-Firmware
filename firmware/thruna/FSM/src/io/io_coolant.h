#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_hal.h"

/**
 * Initializes a given timer handle for a flow meter
 * @param htim : The timer handle for both flow meters
 */
void io_coolant_init(TIM_HandleTypeDef * htim);

/**
 * Update the frequency for the primary flow meter's PWM outputs.
 * @note This function should be called in the input capture callback function.
 * @param htim: The handle of the timer that triggered the input capture
 *              callback function
 */
void io_coolant_inputCaptureCallback(TIM_HandleTypeDef * htim);
#endif

/**
 * @returns the flow rate (L/min) from the primary flow meter
 */
float io_coolant_getFlowRate(void);

/**
 * Check if the PWM signal of the primary flow meter is active. If it is
 * inactive (i.e. It has been unplugged or unpowered), set the frequency to NaN.
 *
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal of the primary flow meter.
 */
void io_coolant_checkIfFlowMeterActive(void);

/**
 * @returns the temperature of the coolant at [sensor B] in Celsius
 */
float io_coolant_getTemperatureA(void);

/**
 * @returns the temperature of the coolant at [sensor B] in Celsius
 */
float io_coolant_getTemperatureB(void);

/**
 * @returns the pressure of the coolant at [sensor A] in PSI
 */
float io_coolant_getPressureA(void);

/**
 * @returns the pressure of the coolant at [sensor B] in PSI
 */
float io_coolant_getPressureB(void);
