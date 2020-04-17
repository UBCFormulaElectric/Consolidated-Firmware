#include <gtest/gtest.h>
#include "Test_Bms.h"

DEFINE_FFF_GLOBALS;

void BmsTest::SetImdCondition(
    enum Imd_Condition condition,
    float &            fake_pwm_frequency_return_val)
{
    const float mapping[NUM_OF_IMD_CONDITIONS] = {
        [IMD_SHORT_CIRCUIT] = 0.0f,          [IMD_NORMAL] = 10.0f,
        [IMD_UNDERVOLTAGE_DETECTED] = 20.0f, [IMD_SST] = 30.0f,
        [IMD_DEVICE_ERROR] = 40.0f,          [IMD_EARTH_FAULT] = 50.0f,
    };

    fake_pwm_frequency_return_val = mapping[condition];
    App_Imd_Tick(imd);
    ASSERT_EQ(condition, App_Imd_GetCondition(imd));
}
