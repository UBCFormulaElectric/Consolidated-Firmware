#pragma once

#include "Test_Dim.h"

DECLARE_FAKE_VALUE_FUNC(uint32_t, get_position);

class RotarySwitchTest : public virtual testing::Test
{
  protected:
    void SetUp() override;
    void TearDown() override;

    struct RotarySwitch *rotary_switch;
    const uint32_t       NUM_ROTARY_SWITCH_POSITIONS = 6;
};
