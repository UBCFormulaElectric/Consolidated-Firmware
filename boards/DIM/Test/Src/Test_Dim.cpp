#include "Test_Dim.h"

DEFINE_FFF_GLOBALS;

void DimTest::SetUp()
{
    left_seven_seg_display   = App_SevenSegDisplay_Create(set_left_hex_digit);
    middle_seven_seg_display = App_SevenSegDisplay_Create(set_middle_hex_digit);
    right_seven_seg_display  = App_SevenSegDisplay_Create(set_right_hex_digit);
    seven_segment_displays   = App_SevenSegDisplays_Create(
        left_seven_seg_display, middle_seven_seg_display,
        right_seven_seg_display);

    RESET_FAKE(set_right_hex_digit);
    RESET_FAKE(set_middle_hex_digit);
    RESET_FAKE(set_left_hex_digit);
}

void DimTest::TearDown()
{
    ASSERT_TRUE(left_seven_seg_display != NULL);
    ASSERT_TRUE(middle_seven_seg_display != NULL);
    ASSERT_TRUE(right_seven_seg_display != NULL);
    ASSERT_TRUE(seven_segment_displays != NULL);

    App_SevenSegDisplay_Destroy(left_seven_seg_display);
    App_SevenSegDisplay_Destroy(middle_seven_seg_display);
    App_SevenSegDisplay_Destroy(right_seven_seg_display);
    App_SevenSegDisplays_Destroy(seven_segment_displays);

    left_seven_seg_display   = NULL;
    middle_seven_seg_display = NULL;
    right_seven_seg_display  = NULL;
    seven_segment_displays   = NULL;
}
