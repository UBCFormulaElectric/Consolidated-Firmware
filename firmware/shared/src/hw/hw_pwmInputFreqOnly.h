#pragma once

#include <stdbool.h>
#include "hw_hal.h"

typedef struct
{
    float frequency_hz;

    TIM_HandleTypeDef    *htim;
    float                 tim_frequency_hz;
    uint32_t              tim_channel;
    uint32_t              tim_auto_reload_reg;
    HAL_TIM_ActiveChannel tim_active_channel;

    bool     first_tick;
    uint32_t curr_rising_edge;
    uint32_t prev_rising_edge;

    size_t tim_overflow_count;
} PwmInputFreqOnly;

/**
 * Initialize a frequency-only PWM input using the given (hardware) timer
 *
 * @note The given timer must be initialized with Input Capture Direct Mode
 * @param htim: The handle of the timer measuring the PWM input
 * @param htim: The handle of the timer measuring the PWM input
 * @param timer_frequency_hz: The frequency of the timer measuring the PWM input
 * @param tim_channel: The timer channel measuring the PWM input
 * @param tim_auto_reload_reg: Maximum value that the counter can count to
 * @param tim_active_channel: The active timer channel measuring the PWM input
 * @return Pointer to the allocated and initialized PWM input
 */
void hw_pwmInputFreqOnly_init(
    PwmInputFreqOnly     *pwm_input,
    TIM_HandleTypeDef    *htim,
    float                 tim_frequency_hz,
    uint32_t              tim_channel,
    uint32_t              tim_auto_reload_reg,
    HAL_TIM_ActiveChannel tim_active_channel);

/**
 * Get the frequency for the given PWM input
 * @param pwm_input: The PWM input to get frequency for
 * @return The frequency for the given PWM input
 */
float hw_pwmInputFreqOnly_getFrequency(const PwmInputFreqOnly *pwm_input);

/**
 * Get the timer handle for the given PWM input
 * @param pwm_input: The PWM input used to get the timer handle
 * @return The timer handle for the given PWM input
 */
TIM_HandleTypeDef *hw_pwmInputFreqOnly_getTimerHandle(const PwmInputFreqOnly *pwm_input);

/**
 * Get the active timer channel for the given PWM input
 * @param pwm_input: The PWM input used to get the active timer channel
 * @return The active timer channel for the given PWM input
 */
HAL_TIM_ActiveChannel hw_pwmInputFreqOnly_getTimerActiveChannel(const PwmInputFreqOnly *pwm_input);

/**
 * Update the frequency for the given PWM input
 * @param pwm_input: The PWM input to update for
 */
void hw_pwmInputFreqOnly_tick(PwmInputFreqOnly *pwm_input);

/**
 * Check if the given PWM signal is active. If the sensor detects a DC signal
 * set the frequency to 0Hz.
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal
 * @param pwm_input: The PWM input to check for
 */
void hw_pwmInputFreqOnly_checkIfPwmIsActive(PwmInputFreqOnly *pwm_input);
