#include "hw_pwmOutput.hpp"

void hw::PwmOutput::start()
{
    HAL_TIM_PWM_Start(htim, pwm_channel);
}

void hw::PwmOutput::stop()
{
    HAL_TIM_PWM_Stop(htim, pwm_channel);
}

void hw::PwmOutput::setDutyCycle(float duty_cycle_in)
{
    // Constrain duty cycle between 0.0% and 100.0%.
    if (duty_cycle_in < 0.0f || duty_cycle_in > 100.0f)
    {
        duty_cycle_in = 0.0f;
    }
    duty_cycle = duty_cycle_in;

    // Get current ARR value
    const uint32_t arr = __HAL_TIM_GET_AUTORELOAD(htim);
    // Calculate CCR value
    const uint32_t ccr_value = static_cast<uint32_t>((duty_cycle / 100.0f) * (static_cast<float>(arr + 1)));

    // Set duty cycle
    __HAL_TIM_SET_COMPARE(htim, pwm_channel, ccr_value);
}

void hw::PwmOutput::getFrequency(float frequency_hz_in)
{
    frequency_hz = frequency_hz_in;
}