#include <main.h>
#include <math.h>
#include <assert.h>
#include "Io_SharedFrequencyOnlyPwmInput.h"

#define MAX_TIMER_PERIOD_ELAPSED_COUNT 2

struct FreqOnlyPwmInput
{
    float              frequency_hz;
    TIM_HandleTypeDef *htim;
    float              timer_frequency_hz;
    uint32_t           rising_edge_tim_channel;
    bool               first_tick;
    uint32_t           curr_rising_edge;
    uint32_t           prev_rising_edge;
    size_t             elapsed_tick;
};

/**
 * Set the frequency for the given PWM input
 * @param pwm_input: The PWM input to set frequency for
 * @param frequency_hz: Frequency, in Hz
 */
static void Io_SetFrequency(
    struct FreqOnlyPwmInput *pwm_input,
    const float              frequency_hz);

static void Io_SetFrequency(
    struct FreqOnlyPwmInput *const pwm_input,
    const float                    frequency_hz)
{
    assert(frequency_hz >= 0.0f || isnanf(frequency_hz));

    pwm_input->frequency_hz = frequency_hz;
}

struct FreqOnlyPwmInput *Io_SharedFreqOnlyPwmInput_Create(
    TIM_HandleTypeDef *htim,
    float              timer_frequency_hz,
    uint32_t           rising_edge_timer_channel)
{
    assert(htim != NULL);
    static struct FreqOnlyPwmInput pwm_inputs[MAX_NUM_OF_FREQ_ONLY_PWM_INPUTS];
    static size_t                  alloc_index = 0;

    assert(alloc_index < MAX_NUM_OF_FREQ_ONLY_PWM_INPUTS);

    struct FreqOnlyPwmInput *const pwm_input = &pwm_inputs[alloc_index++];

    pwm_input->frequency_hz            = NAN;
    pwm_input->htim                    = htim;
    pwm_input->timer_frequency_hz      = timer_frequency_hz;
    pwm_input->rising_edge_tim_channel = rising_edge_timer_channel;
    pwm_input->prev_rising_edge        = 0U;
    pwm_input->curr_rising_edge        = 0U;
    pwm_input->first_tick              = true;
    pwm_input->elapsed_tick            = 0U;

    HAL_TIM_IC_Start_IT(htim, rising_edge_timer_channel);
    HAL_TIM_Base_Start_IT(htim);

    return pwm_input;
}

void Io_SharedFreqOnlyPwmInput_Tick(struct FreqOnlyPwmInput *const pwm_input)
{
    // Reset elapsed tick counter for the flow meter
    pwm_input->elapsed_tick = 0U;

    if (pwm_input->first_tick)
    {
        pwm_input->curr_rising_edge = HAL_TIM_ReadCapturedValue(
            pwm_input->htim, pwm_input->rising_edge_tim_channel);
        pwm_input->first_tick = false;
    }
    else
    {
        pwm_input->first_tick       = true;
        pwm_input->prev_rising_edge = pwm_input->curr_rising_edge;
        pwm_input->curr_rising_edge = HAL_TIM_ReadCapturedValue(
            pwm_input->htim, pwm_input->rising_edge_tim_channel);

        uint32_t       rising_edge_delta;
        const uint32_t prev_rising_edge = pwm_input->prev_rising_edge;
        const uint32_t curr_rising_edge = pwm_input->curr_rising_edge;

        // Check if the counter value has overflowed
        if (curr_rising_edge > prev_rising_edge)
        {
            rising_edge_delta = curr_rising_edge - prev_rising_edge;
            Io_SetFrequency(
                pwm_input,
                (float)pwm_input->timer_frequency_hz / rising_edge_delta);
        }

        else if (curr_rising_edge < prev_rising_edge)
        {
            rising_edge_delta =
                TIMx_AUTO_RELOAD_REG - prev_rising_edge + curr_rising_edge + 1;
            Io_SetFrequency(
                pwm_input,
                (float)pwm_input->timer_frequency_hz / rising_edge_delta);
        }

        else
        {
            Io_SetFrequency(pwm_input, NAN);
        }
    }
}

void Io_SharedFreqOnlyPwmInput_Elapsed_Tick(
    struct FreqOnlyPwmInput *const pwm_input)
{
    const size_t elapsed_tick_count = ++pwm_input->elapsed_tick;

    if (elapsed_tick_count >= MAX_TIMER_PERIOD_ELAPSED_COUNT)
    {
        Io_SetFrequency(pwm_input, NAN);
    }
}

float Io_SharedFreqOnlyPwmInput_GetFrequency(
    struct FreqOnlyPwmInput *const pwm_input)
{
    return pwm_input->frequency_hz;
}
