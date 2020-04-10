#include <math.h>
#include "App_SharedConstants.h"

// The argument history needs to be long enough for us to test write every
// one of the possible 16x16x16 = 4096 values on the three 7-segment displays
#define FFF_ARG_HISTORY_LEN (size_t)pow((float)NUM_HEX_DIGITS, (float)3)

#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SevenSegDisplays.h"

    DEFINE_FFF_GLOBALS;
    FAKE_VOID_FUNC(set_right_hex_digit, uint8_t);
    FAKE_VOID_FUNC(set_middle_hex_digit, uint8_t);
    FAKE_VOID_FUNC(set_left_hex_digit, uint8_t);
}

class SevenSegDisplaysTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        seven_segment_displays = App_SevenSegDisplays_Create(
            set_left_hex_digit, set_middle_hex_digit, set_right_hex_digit);

        RESET_FAKE(set_right_hex_digit);
        RESET_FAKE(set_middle_hex_digit);
        RESET_FAKE(set_left_hex_digit);
    }
    void TearDown() override
    {
        App_SevenSegDisplays_Destroy(seven_segment_displays);
    }

    struct SevenSegDisplays *seven_segment_displays;
};

TEST_F(SevenSegDisplaysTest, write_hexadecimal_values)
{
    uint8_t input[NUM_SEVEN_SEG_DISPLAYS_DIGITS] = { 0, 0, 0 };
    int     count = 0;

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

                count++;

                App_SevenSegDisplays_SetValue(
                    seven_segment_displays, input,
                    NUM_SEVEN_SEG_DISPLAYS_DIGITS);

                ASSERT_EQ(count, set_left_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_middle_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_right_hex_digit_fake.call_count);

                ASSERT_EQ(left_digit,
                    set_left_hex_digit_fake.arg0_history[count-1]);
                ASSERT_EQ(
                    middle_digit,
                    set_middle_hex_digit_fake.arg0_history[count-1]);
                ASSERT_EQ(
                    right_digit,
                    set_right_hex_digit_fake.arg0_history[count-1]);

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
                App_SevenSegDisplays_SetValue(
                    seven_segment_displays, input,
                    NUM_SEVEN_SEG_DISPLAYS_DIGITS);
            },
            "");
    }
}
