#pragma once

#include "hw_pwmInput.hpp"

namespace hw::pwm
{
// External declarations of the PWM Input objects
extern PwmInput imd_pwm_input;
extern PwmInput evse_pwm_input;

// Initialization function (calls .init() on both objects)
void init(void);
} // namespace hw::pwm