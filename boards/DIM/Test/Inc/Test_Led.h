#pragma once

#include "Test_Dim.h"

class LedTest : public virtual testing::Test
{
  public:
    void TearDownLed(struct Led *&led);

  protected:
    void SetUp() override;
    void TearDown() override;
    struct Led *led;
};
