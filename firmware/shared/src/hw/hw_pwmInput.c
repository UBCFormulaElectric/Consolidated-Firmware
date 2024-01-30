#include <assert.h>
#include <stdlib.h>
#include "hw_pwmInput.h"

static uint8_t pwm_counter = 0;

uint8_t pwm_counter_tick(void)
{
    if (pwm_counter != PWM_TICKS_MAX)
    {
        pwm_counter++;
        return pwm_counter;
    }
    return pwm_counter;
}

void io_pwmInput_init(PwmInput *pwm_input, const PwmInputConfig *config)
{
    assert(pwm_input != NULL);
    assert(config != NULL);
    pwm_input->config       = NULL;
    pwm_input->frequency_hz = 0.0f;
    pwm_input->duty_cycle   = 0.0f;
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
    pwm_counter = 0; // Reset the ticks since the last pwm reading
}

float hw_pwmInput_getDutyCycle(const PwmInput *pwm_input)
{
    return pwm_input->duty_cycle;
}

float hw_pwmInput_getFrequency(const PwmInput *pwm_input)
{
    return pwm_input->frequency_hz;
}
