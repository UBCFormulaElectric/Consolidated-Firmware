#pragma once

#include "Test_Dim.h"

extern "C"
{
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
}

DECLARE_FAKE_VOID_FUNC(set_right_hex_digit, struct SevenSegHexDigit);
DECLARE_FAKE_VOID_FUNC(set_middle_hex_digit, struct SevenSegHexDigit);
DECLARE_FAKE_VOID_FUNC(set_left_hex_digit, struct SevenSegHexDigit);
DECLARE_FAKE_VOID_FUNC(display_value_callback);

class SevenSegDisplaysTest : public testing::Test
{
  protected:
    void SetUp() override;
    void TearDown() override;

    struct SevenSegDisplay * left_seven_seg_display;
    struct SevenSegDisplay * middle_seven_seg_display;
    struct SevenSegDisplay * right_seven_seg_display;
    struct SevenSegDisplays *seven_seg_displays;
};
