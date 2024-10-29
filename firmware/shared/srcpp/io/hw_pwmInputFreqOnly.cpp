#include <stdlib.h>
#include "hw_pwmInputFreqOnly.h"
#include <assert.h>

namespace hw
{
void PwmInputFreqOnly::setFrequency(float frequency)
{
    // TODO: set an assert here

    assert(frequency >= 0);
    frequency_hz = frequency;
}

void PwmInputFreqOnly::init(void)
{
    HAL_TIM_IC_Start_IT(htim, tim_channel);
    HAL_TIM_Base_Start_IT(htim);
}

float PwmInputFreqOnly::getFrequency(void)
{
    return frequency_hz;
}

TIM_HandleTypeDef *PwmInputFreqOnly::getTimerHandle(void)
{
    return htim;
}

HAL_TIM_ActiveChannel PwmInputFreqOnly::getTimerActiveChannel(void)
{
    return tim_active_channel;
}

void PwmInputFreqOnly::tick(void)
{
    // Reset the timer overflow count to indicate that the PWM signal is active
    tim_overflow_count = 0U;

    // We store the counter values captured during two most recent rising edges.
    // The difference between these two counter values is used to compute the
    // frequency of the PWM input signal.
    if (first_tick)
    {
        curr_rising_edge = HAL_TIM_ReadCapturedValue(htim, tim_channel);
        first_tick       = false;
    }
    else
    {
        prev_rising_edge = curr_rising_edge;
        curr_rising_edge = HAL_TIM_ReadCapturedValue(htim, tim_channel);

        uint32_t       rising_edge_delta;
        const uint32_t prev_rising = prev_rising_edge;
        const uint32_t curr_rising = curr_rising_edge;

        if (curr_rising > prev_rising)
        {
            rising_edge_delta = curr_rising - prev_rising;
            setFrequency(tim_frequency_hz / (float)rising_edge_delta);
        }
        else if (curr_rising < prev_rising)
        {
            // Occurs when the counter rolls over
            // This plus one need to be removed (Ill remove it but need to see if was done intetionally based on legacy
            // code)
            rising_edge_delta = tim_auto_reload_reg - prev_rising + curr_rising + 1;
            setFrequency(tim_frequency_hz / (float)rising_edge_delta);
        }
        else
        {
            // Occurs when the counter rolls over (i.e. The PWM frequency being
            // measured is too low), or either when a tick arrives before the
            // counter can upcount (i.e. The PWM frequency being measured is
            // too high)
            setFrequency(0.0f);
        }
    }
}

void PwmInputFreqOnly::checkIfPwmIsActive(void)
{
    if (++tim_overflow_count == 2U)
    {
        setFrequency(0.0f);
    }
}

} // namespace hw