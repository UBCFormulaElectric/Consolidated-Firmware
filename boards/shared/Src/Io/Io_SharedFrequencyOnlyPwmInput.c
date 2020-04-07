#include <main.h>
#include "App_SharedAssert.h"
#include "Io_SharedFrequencyOnlyPwmInput.h"
#include <stdbool.h>

struct FrequencyOnlyPwmInput
{
    float              frequency_hz;
    TIM_HandleTypeDef *htim;
    float              timer_frequency_hz;
    uint32_t           rising_edge_tim_channel;
};

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

    return frequency_only_pwm_input;
}

void Io_SharedFrequencyOnlyPwmInput_Tick(
    struct FrequencyOnlyPwmInput *const frequency_only_pwm_input)
{
    shared_assert(frequency_only_pwm_input != NULL);
    static bool first_tick = true;

    static uint32_t previous_rising_edge, current_rising_edge,
        rising_edge_delta;

    if (first_tick)
    {
        UNUSED(rising_edge_delta);
        current_rising_edge = HAL_TIM_ReadCapturedValue(
            frequency_only_pwm_input->htim,
            frequency_only_pwm_input->rising_edge_tim_channel);
        Io_SetFrequency(frequency_only_pwm_input, 0.0f);
        first_tick = false;
    }

    else
    {
        first_tick           = true;
        previous_rising_edge = current_rising_edge;
        current_rising_edge  = HAL_TIM_ReadCapturedValue(
            frequency_only_pwm_input->htim,
            frequency_only_pwm_input->rising_edge_tim_channel);

        // Check if the counter value has overflowed
        rising_edge_delta =
            (current_rising_edge > previous_rising_edge)
                ? current_rising_edge - previous_rising_edge
                : (current_rising_edge < previous_rising_edge)
                      ? (TIMx_AUTO_RELOAD_REG - previous_rising_edge +
                         current_rising_edge + 1)
                      : 0U;

        if (rising_edge_delta != 0U)
        {
            Io_SetFrequency(
                frequency_only_pwm_input,
                frequency_only_pwm_input->timer_frequency_hz /
                    (rising_edge_delta));
        }
        else
        {
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
