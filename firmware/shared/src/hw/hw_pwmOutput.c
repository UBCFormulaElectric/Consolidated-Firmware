#include "hw_pwmOutput.h"
#include <stdint.h>

void hw_pwmOutput_start(PwmOutput *pwm_output)
{
    HAL_TIM_PWM_Start(pwm_output->htim, pwm_output->pwm_channel);
}

void hw_pwmOutput_stop(PwmOutput *pwm_output)
{
    HAL_TIM_PWM_Stop(pwm_output->htim, pwm_output->pwm_channel);
}

void hw_pwmOutput_setDutyCycle(PwmOutput *pwm_output, float duty_cycle)
{
    // Constrain duty cycle between 0.0% and 100.0%.
    if (duty_cycle < 0.0f || duty_cycle > 100)
    {
        duty_cycle = 0.0f;
    }
    pwm_output->duty_cycle = duty_cycle;

    // Get current ARR value
    const uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pwm_output->htim);
    // Calculate CCR value
    const uint32_t ccr_value = (uint32_t)((duty_cycle / 100.0f) * ((float)(arr + 1)));

    // Set duty cycle
    __HAL_TIM_SET_COMPARE(pwm_output->htim, pwm_output->pwm_channel, ccr_value);
}

float hw_pwmOutput_getFrequency(PwmOutput *pwm_output)
{
    return pwm_output->frequency_hz;
}
