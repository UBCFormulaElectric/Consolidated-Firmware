extern "C"
{
// The argument history needs to be long enough for us to test write every
// one of the possible 16x16x16 = 4096 values on the three 7-segment displays
#include <math.h>
#include "App_SharedConstants.h"
#define FFF_ARG_HISTORY_LEN (size_t) pow((float)NUM_HEX_DIGITS, (float)3)
}

#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
#include "App_ErrorCode.h"

    DEFINE_FFF_GLOBALS;
    FAKE_VOID_FUNC(set_right_hex_digit, struct SevenSegHexDigit);
    FAKE_VOID_FUNC(set_middle_hex_digit, struct SevenSegHexDigit);
    FAKE_VOID_FUNC(set_left_hex_digit, struct SevenSegHexDigit);
}

class SevenSegDisplaysTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        left_seven_seg_display = App_SevenSegDisplay_Create(set_left_hex_digit);
        middle_seven_seg_display =
            App_SevenSegDisplay_Create(set_middle_hex_digit);
        right_seven_seg_display =
            App_SevenSegDisplay_Create(set_right_hex_digit);

        seven_segment_displays = App_SevenSegDisplays_Create(
            left_seven_seg_display, middle_seven_seg_display,
            right_seven_seg_display);

        RESET_FAKE(set_right_hex_digit);
        RESET_FAKE(set_middle_hex_digit);
        RESET_FAKE(set_left_hex_digit);
    }

    void TearDown() override
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

    struct SevenSegDisplays *seven_segment_displays;
    struct SevenSegDisplay * left_seven_seg_display;
    struct SevenSegDisplay * middle_seven_seg_display;
    struct SevenSegDisplay * right_seven_seg_display;
};

TEST_F(SevenSegDisplaysTest, write_one_hexadecimal_values)
{
    // Counter used as index for the argument history of fake functions
    int count = 0;

    constexpr size_t input_size        = 1;
    uint8_t          input[input_size] = { 0 };

    for (input[0] = 0; input[0] < NUM_HEX_DIGITS; input[0]++)
    {
        App_SevenSegDisplays_SetHexDigits(
            seven_segment_displays, input, input_size);

        count++;

        ASSERT_EQ(count, set_left_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_middle_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_right_hex_digit_fake.call_count);

        ASSERT_EQ(
            true, set_left_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(
            false, set_middle_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(
            false, set_right_hex_digit_fake.arg0_history[count - 1].enabled);

        ASSERT_EQ(
            input[LEFT_SEVEN_SEG_DISPLAY],
            set_left_hex_digit_fake.arg0_history[count - 1].value);
    }
}

TEST_F(SevenSegDisplaysTest, write_two_hexadecimal_values)
{
    // Counter used as index for the argument history of fake functions
    int count = 0;

    constexpr size_t input_size        = 2;
    uint8_t          input[input_size] = { 0, 0 };

    for (input[0] = 0; input[0] < NUM_HEX_DIGITS; input[0]++)
    {
        for (input[1] = 0; input[1] < NUM_HEX_DIGITS; input[1]++)
        {
            App_SevenSegDisplays_SetHexDigits(
                seven_segment_displays, input, input_size);

            count++;

            ASSERT_EQ(count, set_left_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_middle_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_right_hex_digit_fake.call_count);

            ASSERT_EQ(
                true, set_left_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(
                true,
                set_middle_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(
                false,
                set_right_hex_digit_fake.arg0_history[count - 1].enabled);

            ASSERT_EQ(
                input[0],
                set_left_hex_digit_fake.arg0_history[count - 1].value);
            ASSERT_EQ(
                input[1],
                set_middle_hex_digit_fake.arg0_history[count - 1].value);
        }
    }
}

TEST_F(SevenSegDisplaysTest, write_three_hexadecimal_values)
{
    // Counter used as index for the argument history of fake functions
    int count = 0;

    constexpr size_t input_size        = 3;
    uint8_t          input[input_size] = { 0, 0, 0 };

    for (input[0] = 0; input[0] < NUM_HEX_DIGITS; input[0]++)
    {
        for (input[1] = 0; input[1] < NUM_HEX_DIGITS; input[1]++)
        {
            for (input[2] = 0; input[2] < NUM_HEX_DIGITS; input[2]++)
            {
                App_SevenSegDisplays_SetHexDigits(
                    seven_segment_displays, input, input_size);

                count++;

                ASSERT_EQ(count, set_left_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_middle_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_right_hex_digit_fake.call_count);

                ASSERT_EQ(
                    true,
                    set_left_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(
                    true,
                    set_middle_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(
                    true,
                    set_right_hex_digit_fake.arg0_history[count - 1].enabled);

                ASSERT_EQ(
                    input[0],
                    set_left_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(
                    input[1],
                    set_middle_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(
                    input[2],
                    set_right_hex_digit_fake.arg0_history[count - 1].value);

                // Sanity check to make sure the argument history is long enough
                ASSERT_EQ(0, set_left_hex_digit_fake.arg_histories_dropped);
                ASSERT_EQ(0, set_middle_hex_digit_fake.arg_histories_dropped);
                ASSERT_EQ(0, set_right_hex_digit_fake.arg_histories_dropped);
            }
        }
    }
}

TEST_F(SevenSegDisplaysTest, set_invalid_value)
{
    constexpr int invalid_input = NUM_HEX_DIGITS;
    uint8_t input[NUM_SEVEN_SEG_DISPLAYS];

    // Set an invalid hex value for each display
    for (size_t i = 0; i < NUM_SEVEN_SEG_DISPLAYS; i++)
    {
        memset(input, 0, sizeof(input));
        input[i] = invalid_input;
        EXPECT_EQ(ERROR_CODE_INVALID_ARGS, App_SevenSegDisplays_SetHexDigits(
                    seven_segment_displays, input, NUM_SEVEN_SEG_DISPLAYS));

    }
}
