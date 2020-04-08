#pragma once

#include <stm32f3xx_hal.h>
#include <stdbool.h>

#include "Io_SharedFrequencyOnlyPwmInputConfig.h"

#define MAX_NUM_OF_FREQ_ONLY_PWM_INPUT 2

#ifndef MAX_NUM_OF_FREQ_ONLY_PWM_INPUT
#error Missing definition: MAX_NUM_OF_FREQ_ONLY_PWM_INPUTS must be defined in Io_SharedFreqOnlyPwmInputConfig.h.
#endif

struct FrequencyOnlyPwmInput;

/**
 * Allocate and initialize a PWM input using the given (hardware) timer
 *
 * @note The given timer must be initialized with a particular case of input
 *       capture mode:
 *       - Input Capture direct mode on Channel A
 *       - Input Capture direct mode on Channel B
 *
 * @htim: The handle of the timer measuring the PWM input
 * @timer_frequency_hz: The frequency of the timer measuring the PWM input
 * @rising_edge_tim_channel: The rising edge channel of the timer measuring the
 * PWM input
 * @return Pointer to the allocated and initialized PWM input
 */
struct FrequencyOnlyPwmInput *Io_SharedFrequencyOnlyPwmInput_Create(
    TIM_HandleTypeDef *htim,
    float              timer_frequency_hz,
    uint32_t           rising_edge_tim_channel);

/**
 * Get the frequency for the given PWM input
 * @param frequency_only_pwm_input: The PWM input to get frequency for
 * @return The frequency for the given PWM input
 */
float Io_SharedFrequencyOnlyPwmInput_GetFrequency(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input);

/**
 * Update the frequency and duty cycle for the given PWM input channel (Channel
 * A)
 * @param pwm_input: The PWM input to update for
 */
void Io_SharedFrequencyOnlyPwmInput_Channel_A_Tick(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input);

/**
 * Update the frequency and duty cycle for the given PWM input channel (Channel
 * B)
 * @param pwm_input: The PWM input to update for
 */
void Io_SharedFrequencyOnlyPwmInput_Channel_B_Tick(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input);

/**
 * Detects whether timer period has elapsed for the timer located on the
 * given PWM input channel (Channel A)
 * @param frequency_only_pwm_input
 */
void Io_SharedFrequencyOnlyPwmInput_Period_Elapsed_Tick_Channel_A(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input);

/**
 * Detects whether timer period has elapsed for the timer located on the
 * given PWM input channel (Channel B)
 * @param frequency_only_pwm_input
 */
void Io_SharedFrequencyOnlyPwmInput_Period_Elapsed_Tick_Channel_B(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input);
