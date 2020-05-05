#pragma once

#include "Test_Dim.h"

class RotarySwitchTest : public virtual testing::Test
{
  protected:
    void SetUp() override;
    void TearDown() override;
    void TearDownRotarySwitch(struct RotarySwitch *&rotary_switch_to_teardown);

    struct RotarySwitch *rotary_switch;
    const uint32_t       DEFAULT_NUM_ROTARY_SWITCH_POSITIONS = 6;
};
