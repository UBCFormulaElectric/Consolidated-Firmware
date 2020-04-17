#pragma once

#include "Test_Bms.h"

extern "C"
{
#include "App_Imd.h"
}

DECLARE_FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
DECLARE_FAKE_VALUE_FUNC(uint32_t, get_seconds_since_power_on);
DECLARE_FAKE_VALUE_FUNC(float, get_pwm_frequency_tolerance);
DECLARE_FAKE_VALUE_FUNC(float, get_pwm_frequency);
