#pragma once

#include "Test_Dim.h"

extern "C"
{
#include "App_RotarySwitch.h"
}

DECLARE_FAKE_VALUE_FUNC(enum DriveMode, get_drive_mode);

class RotarySwitchTest : public virtual testing::Test
{
  protected:
    void SetUp() override;
    void TearDown() override;

    struct RotarySwitch *rotary_switch;
};