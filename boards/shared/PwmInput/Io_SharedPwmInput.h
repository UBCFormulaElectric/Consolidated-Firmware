#pragma once

#include <stm32f3xx_hal.h>

// Application specific configuration options.
#include "Io_SharedPwmInputConfig.h"

// Check all the required application specific macros have been defined in
// <Io_SharedPwmInputConfig.h>.
#ifndef MAX_NUM_OF_PWM_INPUTS
#error Missing definition: MAX_NUM_OF_PWM_INPUTS must be defined in Io_SharedPwmInputConfig.h.
#endif

struct PwmInput;

/**
 * Allocate and initialize a PWM input
 * @htim: The handle of the timer measuring the PWM input
 * @timer_frequency_hz: The frequency of the timer measuring the PWM input
 * @rising_edge_tim_channel: The rising edge channel of the timer measuring the
 * PWM input
 * @falling_edge_tim_channel: The falling edge channel of the timer measuring
 * the PWM input
 * @return Pointer to the allocated and initialized PWM input
 */
struct PwmInput *Io_SharedPwmInput_Create(
    TIM_HandleTypeDef *htim,
    uint32_t           timer_frequency_hz,
    uint32_t           rising_edge_tim_channel,
    uint32_t           falling_edge_tim_channel);

/**
 * Update the frequency and duty cycle for the given PWM input
 * @param pwm_input: The PWM input to update for
 */
void Io_SharedPwmInput_Tick(struct PwmInput *const pwm_input);

/**
 * Get the duty cycle for the given PWM input
 * @param pwm_input: The PWM input to get duty cycle for
 * @return The duty cycle for the given PWM input
 */
float Io_SharedPwmInput_GetDutyCycle(const struct PwmInput *const pwm_input);

/**
 * Get the frequency for the given PWM input
 * @param pwm_input: The PWM input to get frequency for
 * @return The frequency for the given PWM input
 */
float Io_SharedPwmInput_GetFrequency(const struct PwmInput *const pwm_input);
