#pragma once

#include "hw_hal.hpp"
#include <hw_pwmOutput.h>
#include <cstdint>

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

namespace hw
{
class PwmOutput
{
  private:
    TIM_HandleTypeDef *htim;
    uint32_t           pwm_channel;
    float              duty_cycle;   // Duty cycle as a percent (0.0 to 100.0)
    float              frequency_hz; // Desired PWM frequency

  public:
    consteval explicit PwmOutput(
        TIM_HandleTypeDef *htim_in,
        uint32_t           pwm_channel_in,
        float              duty_cycle_in,
        float              frequency_hz_in)
      : htim(htim_in), pwm_channel(pwm_channel_in), duty_cycle(duty_cycle_in), frequency_hz(frequency_hz_in)
    {
    }
    /**
     * @brief Start the PWM output.
     */
    void start();

    /**
     * @brief Stop the PWM output.
     */
    void stop();

    /**
     * @brief Set the duty cycle of the PWM output.
     * @param duty_cycle The duty cycle as a percent (0.0 to 100.0).
     */
    void setDutyCycle(float duty_cycle);

    /**
     * @brief Set the frequency of the PWM output.
     * @param frequency_hz The desired PWM frequency in Hz.
     */
    void getFrequency(float frequency_hz);
};
} // namespace hw