#include "hw_pwmInput.h"
#include <stdlib.h>
#include <cstdint> //TODO remove but for now I am getting annoying warnings

namespace hw
{
void PwmInputs::init()
{
    //TODO: put some assets in here
    HAL_TIM_IC_Start_IT(htim, rising_edge_tim_channel);
    HAL_TIM_IC_Start_IT(htim, falling_edge_tim_channel);
}

void  PwmInputs::tick()
{
    uint32_t ic_rising_edge = 
        HAL_TIM_ReadCapturedValue(htim, rising_edge_tim_channel);

    if (ic_rising_edge != 0U)
    {
        uint32_t ic_falling_edge = 
            HAL_TIM_ReadCapturedValue(htim, falling_edge_tim_channel);
        
        duty_cycle = (float)ic_falling_edge * 100.0f / (float)ic_rising_edge;
        frequency_hz = timer_frequency_hz / (float) ic_rising_edge;
    }
    else
    {
        duty_cycle = 0.0f;
        frequency_hz = 0.0f;
    }
}

float PwmInputs::getDutyCycle()
{
    return duty_cycle;
}

float PwmInputs::getFrequncy()
{
    return frequency_hz;
}

}