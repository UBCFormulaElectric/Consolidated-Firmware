#pragma once 
#include "hw_pwmInput.hpp"

namespace hw::pwm
{
extern PwmInput flow_meter_config;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
}