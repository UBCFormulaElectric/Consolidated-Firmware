#include "hw_pwmInput.hpp"
#include "hw_pwmInput.h"
#include <cassert>
#include <stm32f4xx_hal_tim.h>
#include <tuple>

namespace hw
{
void PwmInput::init()
{
    assert(htim != NULL);

    HAL_TIM_IC_Start_IT(htim, rising_edge_tim_channel);
    HAL_TIM_IC_Start_IT(htim, falling_edge_tim_channel);

    HAL_TIM_Base_Start_IT(htim);
}

float PwmInput::get_frequency(void) const
{
    return frequency_hz;
}

float PwmInput::get_dutyCycle(void) const
{
    return duty_cycle;
}

void PwmInput::tick(void)
{
    // Reset the timer overflow count to indicate that the PWM signal is active
    tim_overflow_count = 0;

    // We store the counter values captured during two most recent rising edges.
    // The difference between these two counter values is used to compute the
    // frequency of the PWM input signal.
    if (first_tick)
    {
        curr_rising_edge = HAL_TIM_ReadCapturedValue(htim, rising_edge_tim_channel);
        first_tick       = false;
    }
    else
    {
        prev_rising_edge = curr_rising_edge;

        curr_rising_edge = HAL_TIM_ReadCapturedValue(htim, rising_edge_tim_channel);

        uint32_t rising_edge_detlta;

        if (curr_rising_edge > prev_rising_edge)
        {
            rising_edge_detlta = curr_rising_edge - prev_rising_edge;
            PwmInput::setFrequency(tim_frequency_hz / (float)rising_edge_detlta);
        }
        else if (curr_rising_edge < prev_rising_edge)
        {
            // Occurs when the counter rolls over
            rising_edge_detlta = tim_auto_reload_reg - prev_rising_edge + curr_rising_edge;
            PwmInput::setFrequency(tim_frequency_hz / (float)rising_edge_detlta);
        }
        else
        {
            // Occurs when the counter rolls over (i.e. The PWM frequency being
            // measured is too low), or either when a tick arrives before the
            // counter can upcount (i.e. The PWM frequency being measured is
            // too high)
            PwmInput::setFrequency(0);
        }
    }

    if (mode == PwmMode::PWMINPUT){
        // Calculating duty cycle
        if (curr_rising_edge != 0)
        {
            uint32_t falling_edge = HAL_TIM_ReadCapturedValue(htim, falling_edge_tim_channel);

            duty_cycle = 100.0f * (float)falling_edge / (float)curr_rising_edge;
        }
    }
}

//TODO: change this to exit codes
bool PwmInput::pwm_isActive(void)
{
    // If the timer overflows twice without a rising edge, the PWM signal is
    // likely inactive (i.e. DC signal) and its frequency can't be computed
    if (++tim_overflow_count == 2U)
    {
        PwmInput::setFrequency(0);
        return false;
    }
    return true;
}
} // namespace hw