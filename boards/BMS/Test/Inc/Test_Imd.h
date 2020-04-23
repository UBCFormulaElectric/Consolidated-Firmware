#pragma once

#include "Test_Bms.h"

extern "C"
{
#include "App_Imd.h"
}

DECLARE_FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
DECLARE_FAKE_VALUE_FUNC(uint32_t, get_seconds_since_power_on);
DECLARE_FAKE_VALUE_FUNC(float, get_pwm_frequency);

class ImdTest : public testing::Test
{
  protected:
    void SetUp() override;
    void TearDown() override;
    void SetImdCondition(
        enum Imd_Condition condition,
        float &            fake_pwm_frequency_return_val);
    void SetPwmFrequencyTolerance(float tolerance);

    struct Imd *imd;
};
