#include <main.h>
#include "App_SharedAssert.h"
#include "Io_SharedFrequencyOnlyPwmInput.h"

#define MAXIMUM_OVERFLOW_COUNT 2

static size_t timer_cnt_reg_overflow_count_a = 0;
static size_t timer_period_elapsed_count_b   = 0;

struct FrequencyOnlyPwmInput
{
    float              frequency_hz;
    TIM_HandleTypeDef *htim;
    float              timer_frequency_hz;
    uint32_t           rising_edge_tim_channel;
};

/**
 * Set the frequency for the given PWM input
 * @param pwm_input: The PWM input to set frequency for
 * @param frequency_hz: Frequency, in Hz
 */
static void Io_SetFrequency(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input,
    const float                         frequency_hz);

static void Io_SetFrequency(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input,
    const float                         frequency_hz)
{
    shared_assert(frequency_only_pwm_input != NULL);
    shared_assert(frequency_hz >= 0.0f);

    frequency_only_pwm_input->frequency_hz = frequency_hz;
}

struct FrequencyOnlyPwmInput *Io_SharedFrequencyOnlyPwmInput_Create(
    TIM_HandleTypeDef *htim,
    float              timer_frequency_hz,
    uint32_t           rising_edge_timer_channel)
{
    shared_assert(htim != NULL);
    static struct FrequencyOnlyPwmInput
                  freq_only_pwm_inputs[MAX_NUM_OF_FREQ_ONLY_PWM_INPUT];
    static size_t alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_FREQ_ONLY_PWM_INPUT);

    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input =
        &freq_only_pwm_inputs[alloc_index++];

    frequency_only_pwm_input->htim               = htim;
    frequency_only_pwm_input->timer_frequency_hz = timer_frequency_hz;
    frequency_only_pwm_input->rising_edge_tim_channel =
        rising_edge_timer_channel;

    HAL_TIM_IC_Start_IT(htim, rising_edge_timer_channel);
    HAL_TIM_Base_Start_IT(htim);

    return frequency_only_pwm_input;
}

void Io_SharedFrequencyOnlyPwmInput_Channel_A_Tick(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input)
{
    shared_assert(frequency_only_pwm_input != NULL);

    // Resets the CNT register period elapsed counter
    timer_cnt_reg_overflow_count_a = 0U;

    static bool     channel_a_first_tick = true;
    static uint32_t channel_a_rising_edge_delta, channel_a_current_rising_edge,
        channel_a_previous_rising_edge;

    if (channel_a_first_tick)
    {
        UNUSED(channel_a_rising_edge_delta);
        channel_a_current_rising_edge = HAL_TIM_ReadCapturedValue(
            frequency_only_pwm_input->htim,
            frequency_only_pwm_input->rising_edge_tim_channel);
        channel_a_first_tick = false;
    }

    else
    {
        channel_a_first_tick           = true;
        channel_a_previous_rising_edge = channel_a_current_rising_edge;
        channel_a_current_rising_edge  = HAL_TIM_ReadCapturedValue(
            frequency_only_pwm_input->htim,
            frequency_only_pwm_input->rising_edge_tim_channel);

        // Check if the counter value has overflowed
        channel_a_rising_edge_delta =
            (channel_a_current_rising_edge > channel_a_previous_rising_edge)
                ? channel_a_current_rising_edge - channel_a_previous_rising_edge
                : (channel_a_current_rising_edge <
                   channel_a_previous_rising_edge)
                      ? (TIMx_AUTO_RELOAD_REG - channel_a_previous_rising_edge +
                         channel_a_current_rising_edge + 1)
                      : 0U;

        // Checks if rising_edge_delta is equal to 0
        if (channel_a_rising_edge_delta != 0U)
        {
            Io_SetFrequency(
                frequency_only_pwm_input,
                (float)frequency_only_pwm_input->timer_frequency_hz /
                    channel_a_rising_edge_delta);
        }
        else
        {
            // TODO: How to handle cases where the difference is 0
            Io_SetFrequency(frequency_only_pwm_input, 0.0f);
        }
    }
}

void Io_SharedFrequencyOnlyPwmInput_Channel_B_Tick(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input)
{
    shared_assert(frequency_only_pwm_input != NULL);

    // Resets the CNT register period elapsed counter
    timer_period_elapsed_count_b = 0U;

    static bool     channel_b_first_tick = true;
    static uint32_t channel_b_rising_edge_delta, channel_b_current_rising_edge,
        channel_b_previous_rising_edge;

    if (channel_b_first_tick)
    {
        UNUSED(channel_b_rising_edge_delta);
        channel_b_current_rising_edge = HAL_TIM_ReadCapturedValue(
            frequency_only_pwm_input->htim,
            frequency_only_pwm_input->rising_edge_tim_channel);
        channel_b_first_tick = false;
    }

    else
    {
        channel_b_first_tick           = true;
        channel_b_previous_rising_edge = channel_b_current_rising_edge;
        channel_b_current_rising_edge  = HAL_TIM_ReadCapturedValue(
            frequency_only_pwm_input->htim,
            frequency_only_pwm_input->rising_edge_tim_channel);

        // Check if the counter value has overflowed
        channel_b_rising_edge_delta =
            (channel_b_current_rising_edge > channel_b_previous_rising_edge)
                ? channel_b_current_rising_edge - channel_b_previous_rising_edge
                : (channel_b_current_rising_edge <
                   channel_b_previous_rising_edge)
                      ? (TIMx_AUTO_RELOAD_REG - channel_b_previous_rising_edge +
                         channel_b_current_rising_edge + 1)
                      : 0U;

        // Checks if rising_edge_delta is equal to 0
        if (channel_b_rising_edge_delta != 0U)
        {
            Io_SetFrequency(
                frequency_only_pwm_input,
                (float)frequency_only_pwm_input->timer_frequency_hz /
                    channel_b_rising_edge_delta);
        }
        else
        {
            // TODO: How to handle cases where the difference is 0
            Io_SetFrequency(frequency_only_pwm_input, 0.0f);
        }
    }
}

float Io_SharedFrequencyOnlyPwmInput_GetFrequency(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input)
{
    shared_assert(frequency_only_pwm_input != NULL);
    return frequency_only_pwm_input->frequency_hz;
}

void Io_SharedFrequencyOnlyPwmInput_Period_Elapsed_Tick_Channel_A(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input)
{
    shared_assert(frequency_only_pwm_input != NULL);
    timer_cnt_reg_overflow_count_a++;

    if (timer_cnt_reg_overflow_count_a >= MAXIMUM_OVERFLOW_COUNT)
    {
        Io_SetFrequency(frequency_only_pwm_input, 0.0f);
    }
}

void Io_SharedFrequencyOnlyPwmInput_Period_Elapsed_Tick_Channel_B(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input)
{
    shared_assert(frequency_only_pwm_input != NULL);
    timer_period_elapsed_count_b++;

    if (timer_period_elapsed_count_b >= MAXIMUM_OVERFLOW_COUNT)
    {
        Io_SetFrequency(frequency_only_pwm_input, 0.0f);
    }
}
