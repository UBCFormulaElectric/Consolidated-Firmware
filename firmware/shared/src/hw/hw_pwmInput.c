#include <assert.h>
#include <stdlib.h>
#include "hw_pwmInput.h"

void io_pwmInput_init(PwmInput *pwm_input, const PwmInputConfig *config)
{
    assert(pwm_input != NULL);
    assert(config != NULL);
    pwm_input->config       = config;
    pwm_input->frequency_hz = 0.0f;
    pwm_input->duty_cycle   = 0.0f;

    HAL_TIM_IC_Start_IT(config->htim, config->rising_edge_tim_channel);
    HAL_TIM_IC_Start_IT(config->htim, config->falling_edge_tim_channel);
}

void hw_pwmInput_tick(PwmInput *pwm_input)
{
    uint32_t ic_rising_edge =
        HAL_TIM_ReadCapturedValue(pwm_input->config->htim, pwm_input->config->rising_edge_tim_channel);

    if (ic_rising_edge != 0U)
    {
        uint32_t ic_falling_edge =
            HAL_TIM_ReadCapturedValue(pwm_input->config->htim, pwm_input->config->falling_edge_tim_channel);

        pwm_input->duty_cycle   = (float)ic_falling_edge * 100.0f / (float)ic_rising_edge;
        pwm_input->frequency_hz = pwm_input->config->timer_frequency_hz / (float)ic_rising_edge;
    }
    else
    {
        pwm_input->duty_cycle   = 0.0f;
        pwm_input->frequency_hz = 0.0f;
    }
}

float hw_pwmInput_getDutyCycle(const PwmInput *pwm_input)
{
    return pwm_input->duty_cycle;
}

float hw_pwmInput_getFrequency(const PwmInput *pwm_input)
{
    return pwm_input->frequency_hz;
}
