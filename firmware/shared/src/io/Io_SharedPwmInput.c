#include <assert.h>
#include <stdlib.h>
#include "Io_SharedPwmInput.h"

struct PwmInput
{
    float              duty_cycle;
    float              frequency_hz;
    TIM_HandleTypeDef *htim;
    float              timer_frequency_hz;
    uint32_t           rising_edge_tim_channel;
    uint32_t           falling_edge_tim_channel;
};

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

static void Io_SetDutyCycle(struct PwmInput *const pwm_input, const float duty_cycle)
{
    assert(duty_cycle >= 0.0f);

    pwm_input->duty_cycle = duty_cycle;
}

static void Io_SetFrequency(struct PwmInput *const pwm_input, const float frequency_hz)
{
    assert(frequency_hz >= 0.0f);

    pwm_input->frequency_hz = frequency_hz;
}

struct PwmInput *Io_SharedPwmInput_Create(
    TIM_HandleTypeDef *htim,
    float              timer_frequency_hz,
    uint32_t           rising_edge_tim_channel,
    uint32_t           falling_edge_tim_channel)
{
    assert(htim != NULL);

    struct PwmInput *const pwm_input = malloc(sizeof(struct PwmInput));
    assert(pwm_input != NULL);

    pwm_input->htim                     = htim;
    pwm_input->timer_frequency_hz       = timer_frequency_hz;
    pwm_input->rising_edge_tim_channel  = rising_edge_tim_channel;
    pwm_input->falling_edge_tim_channel = falling_edge_tim_channel;

    HAL_TIM_IC_Start_IT(htim, rising_edge_tim_channel);
    HAL_TIM_IC_Start_IT(htim, falling_edge_tim_channel);

    return pwm_input;
}

void Io_SharedPwmInput_Tick(struct PwmInput *const pwm_input)
{
    uint32_t ic_rising_edge = HAL_TIM_ReadCapturedValue(pwm_input->htim, pwm_input->rising_edge_tim_channel);

    if (ic_rising_edge != 0U)
    {
        uint32_t ic_falling_edge = HAL_TIM_ReadCapturedValue(pwm_input->htim, pwm_input->falling_edge_tim_channel);

        Io_SetDutyCycle(pwm_input, (float)ic_falling_edge * 100.0f / (float)ic_rising_edge);

        Io_SetFrequency(pwm_input, pwm_input->timer_frequency_hz / (float)ic_rising_edge);
    }
    else
    {
        Io_SetDutyCycle(pwm_input, 0.0f);
        Io_SetFrequency(pwm_input, 0.0f);
    }
}

float Io_SharedPwmInput_GetDutyCycle(const struct PwmInput *const pwm_input)
{
    return pwm_input->duty_cycle;
}

float Io_SharedPwmInput_GetFrequency(const struct PwmInput *const pwm_input)
{
    return pwm_input->frequency_hz;
}
