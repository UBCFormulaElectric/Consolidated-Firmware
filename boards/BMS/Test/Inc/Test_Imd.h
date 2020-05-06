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
        struct Imd *       imd,
        enum Imd_Condition condition,
        float &            fake_pwm_frequency_return_val);
    static void SetPwmFrequencyTolerance(struct Imd *imd, float tolerance);

  protected:
    void SetUp() override;
    void TearDown() override;

    struct Imd *imd;
};
