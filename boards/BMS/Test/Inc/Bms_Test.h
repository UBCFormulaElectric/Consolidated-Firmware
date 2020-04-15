#pragma once

#include <gtest/gtest.h>

extern "C"
{
#include "App_Imd.h"
}

class BmsTest : public testing::Test
{
  protected:
    struct Imd *imd;
    void SetImdCondition(float &fff_return_val, enum Imd_Condition condition)
    {
        const float mapping[NUM_OF_IMD_CONDITIONS] = {
            [IMD_SHORT_CIRCUIT] = 0.0f,          [IMD_NORMAL] = 10.0f,
            [IMD_UNDERVOLTAGE_DETECTED] = 20.0f, [IMD_SST] = 30.0f,
            [IMD_DEVICE_ERROR] = 40.0f,          [IMD_EARTH_FAULT] = 50.0f,
        };

        fff_return_val = mapping[condition];
        App_Imd_Tick(imd);
        ASSERT_EQ(condition, App_Imd_GetCondition(imd));
    }
};
