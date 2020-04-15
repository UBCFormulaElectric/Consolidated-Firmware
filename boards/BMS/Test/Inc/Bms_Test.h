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
    void SetImdCondition(float &fff_return_val, enum Imd_Condition condition);
};
