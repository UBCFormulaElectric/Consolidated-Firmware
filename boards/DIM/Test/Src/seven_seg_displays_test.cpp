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
        App_SevenSegDisplay_Destroy(left_seven_seg_display);
        App_SevenSegDisplay_Destroy(middle_seven_seg_display);
        App_SevenSegDisplay_Destroy(right_seven_seg_display);
        App_SevenSegDisplays_Destroy(seven_segment_displays);
    }

    struct SevenSegDisplays *seven_segment_displays;
    struct SevenSegDisplay * left_seven_seg_display;
    struct SevenSegDisplay * middle_seven_seg_display;
    struct SevenSegDisplay * right_seven_seg_display;
};

TEST_F(SevenSegDisplaysTest, write_hexadecimal_values)
{
    // Counter used as index for the argument history of fake functions
    int count = 0;

    uint8_t input[NUM_SEVEN_SEG_DISPLAYS_DIGITS] = { 0, 0, 0 };

    for (size_t left_digit = 0; left_digit < NUM_HEX_DIGITS; left_digit++)
    {
        input[SEVEN_SEG_DISPLAYS_LEFT_HEX_DIGIT] = left_digit;

        for (size_t middle_digit = 0; middle_digit < NUM_HEX_DIGITS;
             middle_digit++)
        {
            input[SEVEN_SEG_DISPLAYS_MIDDLE_HEX_DIGIT] = middle_digit;

            for (size_t right_digit = 0; right_digit < NUM_HEX_DIGITS;
                 right_digit++)
            {
                input[SEVEN_SEG_DISPLAYS_RIGHT_HEX_DIGIT] = right_digit;

                App_SevenSegDisplays_SetHexDigits(
                    seven_segment_displays, input,
                    NUM_SEVEN_SEG_DISPLAYS_DIGITS);

                count++;

                ASSERT_EQ(count, set_left_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_middle_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_right_hex_digit_fake.call_count);

                struct SevenSegHexDigit _left_digit = {
                    .enabled = true, .value = (enum HexDigit)left_digit
                };
                struct SevenSegHexDigit _middle_digit = {
                    .enabled = true, .value = (enum HexDigit)middle_digit
                };
                struct SevenSegHexDigit _right_digit = {
                    .enabled = true, .value = (enum HexDigit)right_digit
                };

                ASSERT_EQ(
                    _left_digit.enabled,
                    set_left_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(
                    _middle_digit.enabled,
                    set_middle_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(
                    _right_digit.enabled,
                    set_right_hex_digit_fake.arg0_history[count - 1].enabled);

                ASSERT_EQ(
                    _left_digit.value,
                    set_left_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(
                    _middle_digit.value,
                    set_middle_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(
                    _right_digit.value,
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
    uint8_t input[NUM_SEVEN_SEG_DISPLAYS_DIGITS];

    // Set an invalid hex value for each display
    for (size_t i = 0; i < NUM_SEVEN_SEG_DISPLAYS_DIGITS; i++)
    {
        memset(input, 0, sizeof(input));
        input[i] = NUM_HEX_DIGITS;
        ASSERT_DEATH(
            {
                App_SevenSegDisplays_SetHexDigits(
                    seven_segment_displays, input,
                    NUM_SEVEN_SEG_DISPLAYS_DIGITS);
            },
            "");
    }
}
