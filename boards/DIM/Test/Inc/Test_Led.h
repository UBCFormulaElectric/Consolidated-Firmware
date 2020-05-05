#pragma once

#include "Test_Dim.h"

class LedTest : public virtual testing::Test
{
  protected:
    void        SetUp() override;
    void        TearDown() override;
    void        TearDownLed(struct Led *&led);
    struct Led *led;
};
