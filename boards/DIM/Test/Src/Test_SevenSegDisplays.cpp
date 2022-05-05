#include "Test_Dim.h"

extern "C"
{
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
}

FAKE_VOID_FUNC(set_right_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_middle_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_left_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(display_value_callback);

class SevenSegDisplaysTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        left_seven_seg_display   = App_SevenSegDisplay_Create(set_left_hex_digit);
        middle_seven_seg_display = App_SevenSegDisplay_Create(set_middle_hex_digit);
        right_seven_seg_display  = App_SevenSegDisplay_Create(set_right_hex_digit);
        seven_seg_displays       = App_SevenSegDisplays_Create(
            left_seven_seg_display, middle_seven_seg_display, right_seven_seg_display, display_value_callback);

        RESET_FAKE(set_right_hex_digit);
        RESET_FAKE(set_middle_hex_digit);
        RESET_FAKE(set_left_hex_digit);
        RESET_FAKE(display_value_callback);
    }

    void TearDown() override
    {
        TearDownObject(left_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(middle_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(right_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(seven_seg_displays, App_SevenSegDisplays_Destroy);
    }

    struct SevenSegDisplay * left_seven_seg_display;
    struct SevenSegDisplay * middle_seven_seg_display;
    struct SevenSegDisplay * right_seven_seg_display;
    struct SevenSegDisplays *seven_seg_displays;
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
        ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, input, num_inputs);
        ASSERT_EQ(EXIT_CODE_OK, exit_code);

        ASSERT_EQ(count, set_left_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_middle_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_right_hex_digit_fake.call_count);

        ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(false, set_middle_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(false, set_right_hex_digit_fake.arg0_history[count - 1].enabled);

        // The value for the middle and right digits are not asserted because we
        // are only writing one digit
        ASSERT_EQ(input[LEFT_SEVEN_SEG_DISPLAY], set_left_hex_digit_fake.arg0_history[count - 1].value);
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
            ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, input, num_inputs);
            ASSERT_EQ(EXIT_CODE_OK, exit_code);

            ASSERT_EQ(count, set_left_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_middle_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_right_hex_digit_fake.call_count);

            ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(false, set_right_hex_digit_fake.arg0_history[count - 1].enabled);

            // The value for the right digit is not asserted because we are only
            // writing two digits
            ASSERT_EQ(input[0], set_left_hex_digit_fake.arg0_history[count - 1].value);
            ASSERT_EQ(input[1], set_middle_hex_digit_fake.arg0_history[count - 1].value);
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
                ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, input, num_inputs);
                ASSERT_EQ(EXIT_CODE_OK, exit_code);

                ASSERT_EQ(count, set_left_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_middle_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_right_hex_digit_fake.call_count);

                ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(true, set_right_hex_digit_fake.arg0_history[count - 1].enabled);

                ASSERT_EQ(input[0], set_left_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(input[1], set_middle_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(input[2], set_right_hex_digit_fake.arg0_history[count - 1].value);

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

        ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, input, num_inputs);
        EXPECT_EQ(EXIT_CODE_INVALID_ARGS, exit_code);

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

        ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, input, num_inputs);
        EXPECT_EQ(EXIT_CODE_INVALID_ARGS, exit_code);

        // We should not write to any of the 7 segment displays if any of the
        // inputs is invalid
        ASSERT_EQ(0, set_left_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_middle_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_right_hex_digit_fake.call_count);
    }
}

TEST_F(SevenSegDisplaysTest, set_valid_unsigned_base10_values)
{
    App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 0);
    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_middle_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_right_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[0].value);

    App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 500);
    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_right_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(0, set_middle_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(5, set_right_hex_digit_fake.arg0_history[1].value);

    App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 999);
    ASSERT_EQ(true, set_left_hex_digit_fake.arg0_history[2].enabled);
    ASSERT_EQ(true, set_middle_hex_digit_fake.arg0_history[2].enabled);
    ASSERT_EQ(true, set_right_hex_digit_fake.arg0_history[2].enabled);
    ASSERT_EQ(9, set_left_hex_digit_fake.arg0_history[2].value);
    ASSERT_EQ(9, set_middle_hex_digit_fake.arg0_history[2].value);
    ASSERT_EQ(9, set_right_hex_digit_fake.arg0_history[2].value);
}

TEST_F(SevenSegDisplaysTest, set_invalid_unsigned_base10_values)
{
    ExitCode exit_code = EXIT_CODE_OK;
    exit_code          = App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 1000);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code);

    ASSERT_EQ(0, set_left_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_middle_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_right_hex_digit_fake.call_count);
}

TEST_F(SevenSegDisplaysTest, set_valid_hex_digits_invokes_callback_function)
{
    const uint8_t hex_digits[NUM_SEVEN_SEG_DISPLAYS] = { 1, 2, 3 };

    for (size_t i = 1; i <= NUM_SEVEN_SEG_DISPLAYS; i++)
    {
        ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, hex_digits, i);
        ASSERT_EQ(EXIT_CODE_OK, exit_code);
        ASSERT_EQ(i, display_value_callback_fake.call_count);
    }
}

TEST_F(SevenSegDisplaysTest, set_invalid_hex_digits_does_not_invoke_callback_function)
{
    uint8_t hex_digits[NUM_SEVEN_SEG_DISPLAYS] = { NUM_HEX_DIGITS, NUM_HEX_DIGITS, NUM_HEX_DIGITS };

    for (size_t i = 1; i <= NUM_SEVEN_SEG_DISPLAYS; i++)
    {
        ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, hex_digits, i);
        ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code);
        ASSERT_EQ(0, display_value_callback_fake.call_count);
    }
}

TEST_F(SevenSegDisplaysTest, invalid_num_digits_for_set_hex_digits_does_not_invoke_callback_function)
{
    uint8_t hex_digits[NUM_SEVEN_SEG_DISPLAYS] = { 0 };

    ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, hex_digits, 0);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code);
    ASSERT_EQ(0, display_value_callback_fake.call_count);
}

TEST_F(SevenSegDisplaysTest, set_valid_unsigned_base10_value_invokes_callback_function)
{
    ExitCode exit_code = App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 1);
    ASSERT_EQ(EXIT_CODE_OK, exit_code);
    ASSERT_EQ(1, display_value_callback_fake.call_count);
}

TEST_F(SevenSegDisplaysTest, set_invalid_unsigned_base10_value_does_not_invoke_callback_function)
{
    ExitCode exit_code = App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 1000);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code);
    ASSERT_EQ(0, display_value_callback_fake.call_count);
}
