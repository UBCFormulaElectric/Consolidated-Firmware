#pragma once

#include <stm32f3xx_hal.h>
#include <stdbool.h>

#include "Io_SharedFreqOnlyPwmInputConfig.h"

#ifndef MAX_NUM_OF_FREQ_ONLY_PWM_INPUTS
#define MAX_NUM_OF_FREQ_ONLY_PWM_INPUTS 0
#endif

struct FreqOnlyPwmInput;

/**
 * Allocate and initialize a PWM input using the given (hardware) timer
 *
 * @note The given timer must be initialized with:
 *       - Input Capture direct mode
 *
 * @param htim: The handle of the timer measuring the PWM input
 * @param timer_frequency_hz: The frequency of the timer measuring the PWM input
 * @param rising_edge_tim_channel: The rising edge channel of the timer
 * measuring the PWM input
 * @param timer_auto_reload_reg : Maximum value that the counter can count to
 * @return Pointer to the allocated and initialized PWM input
 */
struct FreqOnlyPwmInput *Io_SharedFreqOnlyPwmInput_Create(
    TIM_HandleTypeDef *htim,
    float              timer_frequency_hz,
    uint32_t           rising_edge_tim_channel,
    uint32_t           timer_auto_reload_reg);

/**
 * Get the frequency for the given PWM input
 * @param pwm_input: The PWM input to get frequency for
 * @return The frequency for the given PWM input
 */
float Io_SharedFreqOnlyPwmInput_GetFrequency(
    struct FreqOnlyPwmInput *pwm_input);

/**
 * Update the frequency for the given PWM input
 *
 * @note The function captures the value of first rising edge, and then captures
 *       the second rising edge from the capture compare unit. The difference
 *       between these two values, rising_edge_delta are computed and used to
 *       compute the frequency of the pwm input signal
 *
 * @param pwm_input: The PWM input to update for
 **/
void Io_SharedFreqOnlyPwmInput_Tick(struct FreqOnlyPwmInput *pwm_input);

/**
 * Increments elapsed tick counter after a period elapsed interrupt occurs
 *
 * @param pwm_input : The PWM input used to update the frequency when
 * captured PWM enters invalid range
 * @param elapsed_count : Stores the number of times the CNT
 * register for TIMx overflows
 * @return The updated number of times the CNT register for TIMx overflows
 */
void Io_SharedFreqOnlyPwmInput_Elapsed_Tick(struct FreqOnlyPwmInput *pwm_input);
