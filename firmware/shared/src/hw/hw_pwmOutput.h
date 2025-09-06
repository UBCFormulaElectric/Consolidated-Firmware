#pragma once

#include "hw_hal.h"

/*
 * The PWM output driver is designed to generate a PWM signal on a specific timer channel.
 * The timer must be configured in PWM mode prior to using this driver.
 *
 * PWM output driver configuration parameters:
 *
 * - htim: The handle of the timer generating the PWM output.
 * - pwm_channel: The channel of the timer used for the PWM output.
 * - frequency_hz: The frequency of the PWM signal.
 */

typedef struct
{
    TIM_HandleTypeDef *htim;
    uint32_t           pwm_channel;
    float              duty_cycle;   // Duty cycle as a percent (0.0 to 100.0)
    float              frequency_hz; // Desired PWM frequency
} PwmOutput;

/**
 * @brief Start the PWM output.
 * @param pwm_output: PWM output instance.
 */
void hw_pwmOutput_start(PwmOutput *pwm_output);

/**
 * @brief Stop the PWM output.
 * @param pwm_output: PWM output instance.
 */
void hw_pwmOutput_stop(PwmOutput *pwm_output);

/**
 * @brief Set the duty cycle for the given PWM output.
 * @param pwm_output: PWM output instance.
 * @param duty_cycle: Duty cycle in percent (0.0 to 100.0).
 */
void hw_pwmOutput_setDutyCycle(PwmOutput *pwm_output, float duty_cycle);

/**
 * @brief Get the frequency for the given PWM output.
 * @param pwm_output: PWM output instance.
 * @param frequency_hz: Desired PWM frequency.
 */
float hw_pwmOutput_getFrequency(PwmOutput *pwm_output);
