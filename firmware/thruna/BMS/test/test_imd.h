#pragma once

#include <gtest/gtest.h>

#include "fake_io_imd.hpp"

extern "C"
{
#include "app_imd.h"
}

class ImdTest : public testing::Test
{
  public:
    static void SetImdCondition(ImdConditionName condition_name);

  protected:
    void SetUp() override
    {
        fake_io_imd_getFrequency_reset();
        fake_io_imd_getDutyCycle_reset();
    }
};

static void test_imd_setImdCondition(ImdConditionName condition_name)
{
    const float mapping[NUM_OF_IMD_CONDITIONS] = {
        [IMD_SHORT_CIRCUIT] = 0.0f, [IMD_NORMAL] = 10.0f,       [IMD_UNDERVOLTAGE_DETECTED] = 20.0f,
        [IMD_SST] = 30.0f,          [IMD_DEVICE_ERROR] = 40.0f, [IMD_GROUND_FAULT] = 50.0f,
    };

    fake_io_imd_getFrequency_returns(mapping[condition_name]);
    ASSERT_EQ(condition_name, app_imd_getCondition().name);
}
