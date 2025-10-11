#include "hw_pwmInput.hpp"
#include "hw_pwmInput.h"
#include <cassert>
#include <stm32f4xx_hal_tim.h>
#include <tuple>


namespace hw
{
void PwmInput::init(){
    assert(htim != NULL);

    frequency_hz = 0;
    first_tick = false;
    curr_rising_edge = 0;
    prev_rising_edge = 0;
    duty_cycle = 0;
    frequency_hz = 0;

    HAL_TIM_IC_Start_IT(htim, rising_edge_tim_channel);
    HAL_TIM_IC_Start_IT(htim, falling_edge_tim_channel);

    HAL_TIM_Base_Start_IT(htim);
}

float PwmInput::get_frequency() const {
    return frequency_hz;
} 

float PwmInput::get_dutyCycle() const
{
    return duty_cycle;
}
TIM_HandleTypeDef *PwmInput::get_timer_handle() const{
    return htim;
}

HAL_TIM_ActiveChannel PwmInput::get_timer_activeChannel() const{
    return tim_active_channel;
}

void PwmInput::tick(){

    tim_overflow_count = 0;

    if (first_tick)
    {
        curr_rising_edge = HAL_TIM_ReadCapturedValue(htim, rising_edge_tim_channel);
        first_tick = false;
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
        else if(curr_rising_edge < prev_rising_edge)
        {
            rising_edge_detlta = tim_auto_reload_reg - prev_rising_edge + curr_rising_edge;
            PwmInput::setFrequency(tim_frequency_hz / (float)rising_edge_detlta);
        }
        else
        {
            PwmInput::setFrequency(0);
        }
    }

    //Calculating duty cycle
    if (curr_rising_edge != 0)
    {
        uint32_t falling_edge = HAL_TIM_ReadCapturedValue(htim, falling_edge_tim_channel);

        duty_cycle = 100.0f * (float) falling_edge/ (float) curr_rising_edge;
    }
}


//change this to exit codes
bool PwmInput::pwm_isActive() {

    if (++tim_overflow_count == 2U)
    {
        PwmInput::setFrequency(0);
        return false
    }
    return true;
}
}