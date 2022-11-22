#include <assert.h>
#include <stdbool.h>
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

    //freq only
    uint32_t              tim_channel;
    uint32_t              tim_auto_reload_reg;
    HAL_TIM_ActiveChannel tim_active_channel;

    bool     first_tick;
    uint32_t curr_rising_edge;
    uint32_t prev_rising_edge;

    size_t tim_overflow_count;
};

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
    uint32_t           falling_edge_tim_channel,

    uint32_t              tim_channel,
    uint32_t              tim_auto_reload_reg,
    HAL_TIM_ActiveChannel tim_active_channel,
    bool                  freqOnly)
{
    assert(htim != NULL);

    struct PwmInput *const pwm_input = malloc(sizeof(struct PwmInput));
    assert(pwm_input != NULL);

    pwm_input->htim                     = htim;
    pwm_input->timer_frequency_hz       = timer_frequency_hz;
    pwm_input->rising_edge_tim_channel  = rising_edge_tim_channel;
    pwm_input->falling_edge_tim_channel = falling_edge_tim_channel;

    pwm_input->frequency_hz        = 0.0f;
    pwm_input->tim_channel         = tim_channel;
    pwm_input->tim_auto_reload_reg = tim_auto_reload_reg;
    pwm_input->tim_active_channel  = tim_active_channel;
    pwm_input->prev_rising_edge    = 0U;
    pwm_input->curr_rising_edge    = 0U;
    pwm_input->first_tick          = true;
    pwm_input->tim_overflow_count  = 0U;

    if (!freqOnly) {
        HAL_TIM_IC_Start_IT(htim, rising_edge_tim_channel);
        HAL_TIM_IC_Start_IT(htim, falling_edge_tim_channel);
    } else {
        HAL_TIM_IC_Start_IT(htim, tim_channel);
        HAL_TIM_Base_Start_IT(htim);
    }

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

// frequency only 


TIM_HandleTypeDef *Io_SharedPwmInput_GetTimerHandle(const struct PwmInput *const pwm_input)
{
    return pwm_input->htim;
}

HAL_TIM_ActiveChannel Io_SharedPwmInput_GetTimerActiveChannel(const struct PwmInput *const pwm_input)
{
    return pwm_input->tim_active_channel;
}

void Io_SharedPwmInput_Tick(struct PwmInput *const pwm_input)
{
    // Reset the timer overflow count to indicate that the PWM signal is active
    pwm_input->tim_overflow_count = 0U;

    // We store the counter values captured during two most recent rising edges.
    // The difference between these two counter values is used to compute the
    // frequency of the PWM input signal.

    if (pwm_input->first_tick)
    {
        pwm_input->curr_rising_edge = HAL_TIM_ReadCapturedValue(pwm_input->htim, pwm_input->tim_channel);
        pwm_input->first_tick       = false;
    }
    else
    {
        pwm_input->prev_rising_edge = pwm_input->curr_rising_edge;
        pwm_input->curr_rising_edge = HAL_TIM_ReadCapturedValue(pwm_input->htim, pwm_input->tim_channel);

        uint32_t       rising_edge_delta;
        const uint32_t prev_rising_edge = pwm_input->prev_rising_edge;
        const uint32_t curr_rising_edge = pwm_input->curr_rising_edge;

        if (curr_rising_edge > prev_rising_edge)
        {
            rising_edge_delta = curr_rising_edge - prev_rising_edge;
            Io_SetFrequency(pwm_input, pwm_input->timer_frequency_hz / (float)rising_edge_delta);
        }
        else if (curr_rising_edge < prev_rising_edge)
        {
            // Occurs when the counter rolls over
            rising_edge_delta = pwm_input->tim_auto_reload_reg - prev_rising_edge + curr_rising_edge + 1;
            Io_SetFrequency(pwm_input, pwm_input->timer_frequency_hz / (float)rising_edge_delta);
        }
        else
        {
            // Occurs when the counter rolls over (i.e. The PWM frequency being
            // measured is too low), or either when a tick arrives before the
            // counter can upcount (i.e. The PWM frequency being measured is
            // too high)
            Io_SetFrequency(pwm_input, 0.0f);
        }
    }
}

void Io_SharedPwmInput_CheckIfPwmIsActive(struct PwmInput *const pwm_input)
{
    // If the timer overflows twice without a rising edge, the PWM signal is
    // likely inactive (i.e. DC signal) and its frequency can't be computed
    if (++pwm_input->tim_overflow_count == 2U)
    {
        Io_SetFrequency(pwm_input, 0.0f);
    }
}

// TODO: Update Io_WheelSpeedSensors.c and Io_Imd.c dependencies