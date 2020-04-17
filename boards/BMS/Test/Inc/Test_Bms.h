#pragma once

#include <fff.h>
#include <gtest/gtest.h>

extern "C"
{
#include "App_Imd.h"
}

class BmsTest : public testing::Test
{
  protected:
    struct Imd *imd;
    void        SetImdCondition(
               enum Imd_Condition condition,
               float &            fake_pwm_frequency_return_val);
};
