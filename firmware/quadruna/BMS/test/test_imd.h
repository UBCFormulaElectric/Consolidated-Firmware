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

static void test_imd_setImdCondition(const ImdConditionName condition_name)
{
    const std::map<ImdConditionName, float> mapping{
        { IMD_CONDITION_SHORT_CIRCUIT, 0.0f },          { IMD_CONDITION_NORMAL, 10.0f },
        { IMD_CONDITION_UNDERVOLTAGE_DETECTED, 20.0f }, { IMD_CONDITION_SST, 30.0f },
        { IMD_CONDITION_DEVICE_ERROR, 40.0f },          { IMD_CONDITION_GROUND_FAULT, 50.0f }
    };

    fake_io_imd_getFrequency_returns(mapping.at(condition_name));
    ASSERT_EQ(condition_name, app_imd_getCondition().name);
}
