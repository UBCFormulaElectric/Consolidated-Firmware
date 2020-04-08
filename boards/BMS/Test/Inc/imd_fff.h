#pragma once

extern "C"
{
#include <stdint.h>
FAKE_VALUE_FUNC(float, get_pwm_frequency);
FAKE_VALUE_FUNC(float, get_pwm_frequency_tolerance);
FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
FAKE_VALUE_FUNC(uint32_t, get_seconds_since_power_on);
}
