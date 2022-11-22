#pragma once

#include <stm32f3xx_hal.h>

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
 * @param tim_channel: The timer channel measuring the PWM input
 * @param tim_auto_reload_reg: Maximum value that the counter can count to
 * @param tim_active_channel: The active timer channel measuring the PWM input
 * @return Pointer to the allocated and initialized PWM input
 */
struct PwmInput *Io_SharedPwmInput_Create(
    TIM_HandleTypeDef *htim,
    float              timer_frequency_hz,
    uint32_t           rising_edge_tim_channel,
    uint32_t           falling_edge_tim_channel,
    //freq only
    uint32_t              tim_channel,
    uint32_t              tim_auto_reload_reg,
    HAL_TIM_ActiveChannel tim_active_channe,
    bool                  freqOnly);

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

/**
 * Set the duty cycle for the given PWM input
 * @param pwm_input: The PWM input to set duty cycle for
 * @param duty_cycle: Duty cycle, in decimal (e.g. 1% = 0.01f)
 */
static void Io_SetDutyCycle(struct PwmInput *const pwm_input, const float duty_cycle);

/**
 * Set the frequency for the given PWM input
 * @param pwm_input: The PWM input to set frequency for
 * @param frequency_hz: Frequency, in Hz
 */
static void Io_SetFrequency(struct PwmInput *const pwm_input, const float frequency_hz);


/**
 * Get the timer handle for the given PWM input
 * @param pwm_input: The PWM input used to get the timer handle
 * @return The timer handle for the given PWM input
 */
TIM_HandleTypeDef *Io_SharedPwmInput_GetTimerHandle(const struct PwmInput *pwm_input);

/**
 * Get the active timer channel for the given PWM input
 * @param pwm_input: The PWM input used to get the active timer channel
 * @return The active timer channel for the given PWM input
 */
HAL_TIM_ActiveChannel Io_SharedPwmInput_GetTimerActiveChannel(const struct PwmInput *pwm_input);

/**
 * Update the frequency for the given PWM input
 * @param pwm_input: The PWM input to update for
 */
void Io_SharedPwmInput_Tick(struct PwmInput *pwm_input);

/**
 * Check if the given PWM signal is active. If the sensor detects a DC signal
 * set the frequency to 0Hz.
 * @note This function should be called in the timer overflow interrupt
 *       for the PWM signal
 * @param pwm_input: The PWM input to check for
 */
void Io_SharedPwmInput_CheckIfPwmIsActive(struct PwmInput *pwm_input);
