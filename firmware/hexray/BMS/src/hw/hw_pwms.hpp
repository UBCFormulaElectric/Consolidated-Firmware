#pragma once

#include "hw_pwmInput.hpp"

// External declarations of the PWM Input objects
extern const hw::PwmInput imd_pwm_input;
extern const hw::PwmInput evse_pwm_input;

inline void pwms_init()
{
    LOG_IF_ERR(imd_pwm_input.init());
    LOG_IF_ERR(evse_pwm_input.init());
}