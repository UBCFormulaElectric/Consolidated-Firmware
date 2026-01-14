#include "hw_pwmInput.hpp"
#include "main.h"

#define TIM_3_ARR 65535
#define TIM_3_

namespace hw
{
PwmInput pwm(&htim1, HAL_TIM_ACTIVE_CHANNEL_1, );
}