#pragma once

#include "Test_Dim.h"

class LedTest : public virtual testing::Test
{
  public:
    void SetupLed(
        struct Led *&led,
        void (*turn_on_led)(void),
        void (*turn_off_led)(void));
    void TearDownLed(struct Led *&led);

  protected:
    void SetUp() override;
    void TearDown() override;

  private:
    struct Led *prv_led;
};
