#pragma once

#include "Io_Hal.h"

struct PwmInput;

/**
 * Allocate and initialize a PWM input using the given (hardware) timer
 *
 * @note The given timer must be initialized with a particular case of input
 *       capture mode:
 *       - Two ICx signals are mapped on the same TIx input.
 *       - These 2 ICx signals are active on edges with opposite polarity.
 *       - One of the two TIxFP signals is selected as trigger input and the
 *         slave mode controller is configured in reset mode.
 * @note Not all timers support this particular case of input capture mode.
 *       Consult the datasheet to double check!
 *
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
    float              timer_frequency_hz,
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
