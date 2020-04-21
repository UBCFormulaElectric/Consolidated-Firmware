#pragma once

// The argument history needs to be long enough for us to test write every
// one of the possible 16x16x16 = 4096 values on the three 7-segment displays.
#include <math.h>
#include "App_SharedConstants.h"
#define FFF_ARG_HISTORY_LEN (size_t) pow((float)NUM_HEX_DIGITS, (float)3)

#include <fff.h>
#include <gtest/gtest.h>

#include "Test_SevenSegDisplays.h"

extern "C"
{
#include "App_SevenSegDisplay.h"
#include "App_SevenSegDisplays.h"
#include "App_SocDigits.h"
}

class DimTest : public testing::Test
{
  protected:
    struct SevenSegDisplay *  left_seven_seg_display;
    struct SevenSegDisplay *  middle_seven_seg_display;
    struct SevenSegDisplay *  right_seven_seg_display;
    struct SevenSegDisplays * seven_segment_displays;
    struct SocDigits* soc_digits;

    void SetUp() override;
    void TearDown() override;
};
