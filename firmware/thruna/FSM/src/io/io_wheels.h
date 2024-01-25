#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_hal.h"

/**
 * Initialize the left and right wheel speed sensors
 * @param htim_left_wheel_speed Timer handle for the left wheel speed
 * @param htim_right_wheel_speed Timer handle for the right wheel speed
 */
void io_wheels_init(TIM_HandleTypeDef *htim_left_wheel_speed, TIM_HandleTypeDef *htim_right_wheel_speed);

/**
 * Update the frequency for the left and right wheel speed sensors.
 * @note This function should be called in the input capture callback function.
 * @param htim The handle of the timer that triggered the input capture callback
 * function
 */
void io_wheels_inputCaptureCallback(TIM_HandleTypeDef *htim);
#endif

/**
 * Get the wheel speed in km/h from the left wheel speed sensor
 * @return The wheel speed in km/h
 */
float io_wheels_getLeftSpeedKph(void);

/**
 * Get the wheel speed in km/h from the right wheel speed sensor
 * @return The wheel speed in km/h
 */
float io_wheels_getRightSpeedKph(void);

/**
 * Check if the PWM signal of the left wheel speed sensor is active. If
 * it is inactive (i.e. It has been unplugged or unpowered), set the frequency
 * to NaN.
 *
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal of the left wheel speed sensor.
 */
void io_wheels_checkIfLeftSensorActive(void);

/**
 * Check if the PWM signal of the right wheel speed sensor is active. If
 * it is inactive (i.e. It has been unplugged or unpowered), set the frequency
 * to NaN.
 *
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal of the right wheel speed sensor.
 */
void io_wheels_checkIfRightSensorActive(void);
