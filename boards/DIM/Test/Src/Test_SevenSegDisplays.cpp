#include "Test_SevenSegDisplays.h"

DEFINE_FAKE_VOID_FUNC(set_right_hex_digit, struct SevenSegHexDigit);
DEFINE_FAKE_VOID_FUNC(set_middle_hex_digit, struct SevenSegHexDigit);
DEFINE_FAKE_VOID_FUNC(set_left_hex_digit, struct SevenSegHexDigit);

class SevenSegDisplaysTest : public DimTest
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

TEST_F(SevenSegDisplaysTest, set_one_hexadecimal_digit)
{
    // Counter used as index for the argument history and call count of fake
    // functions
    size_t count = 1;

    constexpr size_t num_inputs        = 1;
    uint8_t          input[num_inputs] = { 0 };

    for (input[0] = 0; input[0] < NUM_HEX_DIGITS; input[0]++, count++)
    {
        ErrorCode error_code = App_SevenSegDisplays_SetHexDigits(
            seven_segment_displays, input, num_inputs);
        ASSERT_EQ(ERROR_CODE_OK, error_code);

        ASSERT_EQ(count, set_left_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_middle_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_right_hex_digit_fake.call_count);

        ASSERT_EQ(
            true, set_left_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(
            false, set_middle_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(
            false, set_right_hex_digit_fake.arg0_history[count - 1].enabled);

        // The value for the middle and right digits are not asserted because we
        // are only writing one digit
        ASSERT_EQ(
            input[LEFT_SEVEN_SEG_DISPLAY],
            set_left_hex_digit_fake.arg0_history[count - 1].value);
    }
}

TEST_F(SevenSegDisplaysTest, set_two_hexadecimal_digits)
{
    // Counter used as index for the argument history and call count of fake
    // functions
    size_t count = 1;

    constexpr size_t num_inputs        = 2;
    uint8_t          input[num_inputs] = { 0, 0 };

    for (input[0] = 0; input[0] < NUM_HEX_DIGITS; input[0]++)
    {
        for (input[1] = 0; input[1] < NUM_HEX_DIGITS; input[1]++, count++)
        {
            ErrorCode error_code = App_SevenSegDisplays_SetHexDigits(
                seven_segment_displays, input, num_inputs);
            ASSERT_EQ(ERROR_CODE_OK, error_code);

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

            // The value for the right digit is not asserted because we are only
            // writing two digits
            ASSERT_EQ(
                input[0],
                set_left_hex_digit_fake.arg0_history[count - 1].value);
            ASSERT_EQ(
                input[1],
                set_middle_hex_digit_fake.arg0_history[count - 1].value);
        }
    }
}

TEST_F(SevenSegDisplaysTest, set_three_hexadecimal_digits)
{
    // Counter used as index for the argument history and call count of fake
    // functions
    size_t count = 1;

    constexpr size_t num_inputs        = 3;
    uint8_t          input[num_inputs] = { 0, 0, 0 };

    for (input[0] = 0; input[0] < NUM_HEX_DIGITS; input[0]++)
    {
        for (input[1] = 0; input[1] < NUM_HEX_DIGITS; input[1]++)
        {
            for (input[2] = 0; input[2] < NUM_HEX_DIGITS; input[2]++, count++)
            {
                ErrorCode error_code = App_SevenSegDisplays_SetHexDigits(
                    seven_segment_displays, input, num_inputs);
                ASSERT_EQ(ERROR_CODE_OK, error_code);

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

TEST_F(SevenSegDisplaysTest, set_underflow_digit_to_each_7_seg_display)
{
    constexpr size_t num_inputs = NUM_SEVEN_SEG_DISPLAYS;
    uint8_t          input[num_inputs];

    for (size_t i = 0; i < num_inputs; i++)
    {
        memset(input, 0, sizeof(input));

        constexpr uint8_t underflow_input = HEX_DIGIT_0 - 1;
        input[i]                          = underflow_input;

        ErrorCode error_code = App_SevenSegDisplays_SetHexDigits(
            seven_segment_displays, input, num_inputs);
        EXPECT_EQ(ERROR_CODE_INVALID_ARGS, error_code);

        // We should not write to any of the 7 segment displays if any of the
        // inputs is invalid
        ASSERT_EQ(0, set_left_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_middle_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_right_hex_digit_fake.call_count);
    }
}

TEST_F(SevenSegDisplaysTest, set_overflow_digit_to_each_7_seg_display)
{
    constexpr size_t num_inputs = NUM_SEVEN_SEG_DISPLAYS;
    uint8_t          input[num_inputs];

    for (size_t i = 0; i < num_inputs; i++)
    {
        memset(input, 0, sizeof(input));

        constexpr uint8_t overflow_input = HEX_DIGIT_F + 1;
        input[i]                         = overflow_input;

        ErrorCode error_code = App_SevenSegDisplays_SetHexDigits(
            seven_segment_displays, input, num_inputs);
        EXPECT_EQ(ERROR_CODE_INVALID_ARGS, error_code);

        // We should not write to any of the 7 segment displays if any of the
        // inputs is invalid
        ASSERT_EQ(0, set_left_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_middle_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_right_hex_digit_fake.call_count);
    }
}
