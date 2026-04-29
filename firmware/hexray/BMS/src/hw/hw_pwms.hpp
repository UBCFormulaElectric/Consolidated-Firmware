#pragma once

#include "hw_pwmInput.hpp"

namespace hw::pwm
{
// External declarations of the PWM Input objects
extern const PwmInput imd_pwm_input;
extern const PwmInput evse_pwm_input;

// Initialization function (calls .init() on both objects)
void init();
} // namespace hw::pwm