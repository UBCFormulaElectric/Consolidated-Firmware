#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hw_pwmInputFreqOnly.h"

/**
 * Set the frequency for the given PWM input
 * @param pwm_input: The PWM input to set frequency for
 * @param frequency_hz: Frequency, in Hz
 */
static void setFrequency(PwmInputFreqOnly *pwm_input, float frequency_hz)
{
    assert(frequency_hz >= 0.0f);
    pwm_input->frequency_hz = frequency_hz;
}

void hw_pwmInputFreqOnly_init(
    PwmInputFreqOnly     *pwm_input,
    TIM_HandleTypeDef    *htim,
    float                 tim_frequency_hz,
    uint32_t              tim_channel,
    uint32_t              tim_auto_reload_reg,
    HAL_TIM_ActiveChannel tim_active_channel)
{
    assert(htim != NULL);
    assert(pwm_input != NULL);

    pwm_input->frequency_hz        = 0.0f;
    pwm_input->htim                = htim;
    pwm_input->tim_frequency_hz    = tim_frequency_hz;
    pwm_input->tim_channel         = tim_channel;
    pwm_input->tim_auto_reload_reg = tim_auto_reload_reg;
    pwm_input->tim_active_channel  = tim_active_channel;
    pwm_input->prev_rising_edge    = 0U;
    pwm_input->curr_rising_edge    = 0U;
    pwm_input->first_tick          = true;
    pwm_input->tim_overflow_count  = 0U;

    HAL_TIM_IC_Start_IT(htim, tim_channel);
    HAL_TIM_Base_Start_IT(htim);
}

float hw_pwmInputFreqOnly_getFrequency(const PwmInputFreqOnly *const pwm_input)
{
    return pwm_input->frequency_hz;
}

TIM_HandleTypeDef *hw_pwmInputFreqOnly_getTimerHandle(const PwmInputFreqOnly *const pwm_input)
{
    return pwm_input->htim;
}

HAL_TIM_ActiveChannel hw_pwmInputFreqOnly_getTimerActiveChannel(const PwmInputFreqOnly *const pwm_input)
{
    return pwm_input->tim_active_channel;
}

void hw_pwmInputFreqOnly_tick(PwmInputFreqOnly *const pwm_input)
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
            setFrequency(pwm_input, pwm_input->tim_frequency_hz / (float)rising_edge_delta);
        }
        else if (curr_rising_edge < prev_rising_edge)
        {
            // Occurs when the counter rolls over
            rising_edge_delta = pwm_input->tim_auto_reload_reg - prev_rising_edge + curr_rising_edge + 1;
            setFrequency(pwm_input, pwm_input->tim_frequency_hz / (float)rising_edge_delta);
        }
        else
        {
            // Occurs when the counter rolls over (i.e. The PWM frequency being
            // measured is too low), or either when a tick arrives before the
            // counter can upcount (i.e. The PWM frequency being measured is
            // too high)
            setFrequency(pwm_input, 0.0f);
        }
    }
}

void hw_pwmInputFreqOnly_checkIfPwmIsActive(PwmInputFreqOnly *const pwm_input)
{
    // If the timer overflows twice without a rising edge, the PWM signal is
    // likely inactive (i.e. DC signal) and its frequency can't be computed
    if (++pwm_input->tim_overflow_count == 2U)
    {
        setFrequency(pwm_input, 0.0f);
    }
}
