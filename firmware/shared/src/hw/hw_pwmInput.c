#include <assert.h>
#include <stdlib.h>
#include "hw_pwmInput.h"

/*
This is a software counter that is incremented everytime the status of the IMD
is broadcasted to CAN and the counter resets everytime the pwm signal from the
IMD is received and turned into an IMD state to broadcast. When the counter reaches
255 it will result in a 0 hz reading from the pwm signal. Usually if the IMD was not
sending a frequency, it would not trigger the interrupt that updates IMD state
*/
static uint8_t pwm_counter = 0;

uint8_t hw_pwm_counter_tick(void)
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
