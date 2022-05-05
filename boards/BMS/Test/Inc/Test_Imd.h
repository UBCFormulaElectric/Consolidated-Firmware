#pragma once

#include "Test_Bms.h"

extern "C"
{
#include "App_Imd.h"
}

class ImdTest : public testing::Test
{
  public:
    static void SetImdCondition(
        struct Imd *           imd_to_set,
        enum Imd_ConditionName condition_name,
        float &                fake_pwm_frequency_return_val);
    static void SetPwmFrequencyTolerance(struct Imd *&imd_to_set, float tolerance);

  protected:
    void SetUp() override;
    void TearDown() override;

    struct Imd *imd;
};
