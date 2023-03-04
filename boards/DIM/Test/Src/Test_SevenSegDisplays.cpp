#include "Test_Dim.h"

extern "C"
{
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
}

FAKE_VOID_FUNC(set_right_l_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_right_m_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_right_r_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_middle_l_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_middle_m_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_middle_r_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_left_l_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_left_m_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(set_left_r_hex_digit, struct SevenSegHexDigit);
FAKE_VOID_FUNC(display_value_callback);

class SevenSegDisplaysTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        left_l_seven_seg_display   = App_SevenSegDisplay_Create(set_left_l_hex_digit);
        left_m_seven_seg_display   = App_SevenSegDisplay_Create(set_left_m_hex_digit);
        left_r_seven_seg_display   = App_SevenSegDisplay_Create(set_left_r_hex_digit);
        middle_l_seven_seg_display = App_SevenSegDisplay_Create(set_middle_l_hex_digit);
        middle_m_seven_seg_display = App_SevenSegDisplay_Create(set_middle_m_hex_digit);
        middle_r_seven_seg_display = App_SevenSegDisplay_Create(set_middle_r_hex_digit);
        right_l_seven_seg_display  = App_SevenSegDisplay_Create(set_right_l_hex_digit);
        right_m_seven_seg_display  = App_SevenSegDisplay_Create(set_right_m_hex_digit);
        right_r_seven_seg_display  = App_SevenSegDisplay_Create(set_right_r_hex_digit);


        seven_seg_displays = App_SevenSegDisplays_Create(
                left_l_seven_seg_display, left_m_seven_seg_display, left_r_seven_seg_display, middle_l_seven_seg_display, middle_m_seven_seg_display, middle_r_seven_seg_display,
                right_l_seven_seg_display, right_m_seven_seg_display, right_r_seven_seg_display, display_value_callback);

        RESET_FAKE(set_left_l_hex_digit);
        RESET_FAKE(set_left_m_hex_digit);
        RESET_FAKE(set_left_r_hex_digit);
        RESET_FAKE(set_middle_l_hex_digit);
        RESET_FAKE(set_middle_m_hex_digit);
        RESET_FAKE(set_middle_r_hex_digit);
        RESET_FAKE(set_right_l_hex_digit);
        RESET_FAKE(set_right_m_hex_digit);
        RESET_FAKE(set_right_r_hex_digit);
        RESET_FAKE(display_value_callback);
    }

    void TearDown() override
    {
        TearDownObject(left_l_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(left_m_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(left_r_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(middle_l_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(middle_m_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(middle_r_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(right_l_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(right_m_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(right_r_seven_seg_display, App_SevenSegDisplay_Destroy);
        TearDownObject(seven_seg_displays, App_SevenSegDisplays_Destroy);
    }

    struct SevenSegDisplay * left_l_seven_seg_display;
    struct SevenSegDisplay * left_m_seven_seg_display;
    struct SevenSegDisplay * left_r_seven_seg_display;
    struct SevenSegDisplay * middle_l_seven_seg_display;
    struct SevenSegDisplay * middle_m_seven_seg_display;
    struct SevenSegDisplay * middle_r_seven_seg_display;
    struct SevenSegDisplay * right_l_seven_seg_display;
    struct SevenSegDisplay * right_m_seven_seg_display;
    struct SevenSegDisplay * right_r_seven_seg_display;
    struct SevenSegDisplays *seven_seg_displays;
};


TEST_F(SevenSegDisplaysTest, set_one_hexadecimal_group)
{
    // Counter used as index for the argument history and call count of fake
    // functions
    size_t count = 1;

    constexpr size_t num_inputs        = 3;
    uint8_t          input[num_inputs] = {0,0,0};

    for (input[0] = 0; input[0] < NUM_HEX_DIGITS; input[0]++, count++)
    {
        ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, input, num_inputs);
        ASSERT_EQ(EXIT_CODE_OK, exit_code);

        ASSERT_EQ(count, set_left_l_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_left_m_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_left_r_hex_digit_fake.call_count);

        ASSERT_EQ(count, set_middle_l_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_middle_m_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_middle_r_hex_digit_fake.call_count);

        ASSERT_EQ(count, set_right_l_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_right_m_hex_digit_fake.call_count);
        ASSERT_EQ(count, set_right_r_hex_digit_fake.call_count);


        ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(true, set_left_m_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(true, set_left_r_hex_digit_fake.arg0_history[count - 1].enabled);

        ASSERT_EQ(false, set_middle_l_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(false, set_middle_m_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(false, set_middle_r_hex_digit_fake.arg0_history[count - 1].enabled);

        ASSERT_EQ(false, set_right_l_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(false, set_right_m_hex_digit_fake.arg0_history[count - 1].enabled);
        ASSERT_EQ(false, set_right_r_hex_digit_fake.arg0_history[count - 1].enabled);

        // The value for the middle and right digits are not asserted because we
        // are only writing one digit
        ASSERT_EQ(input[LEFT_L_SEVEN_SEG_DISPLAY], set_left_l_hex_digit_fake.arg0_history[count - 1].value);
    }
}

TEST_F(SevenSegDisplaysTest, set_two_hexadecimal_groups)
{
    // Counter used as index for the argument history and call count of fake
    // functions
    size_t count = 1;

    constexpr size_t num_inputs        = 6;
    uint8_t          input[num_inputs] = { 0, 0,0,0,0,0};

    for (input[0] = 0,input[1] = 0, input[2] = 0; input[0] < NUM_HEX_DIGITS; input[0]++, input[1]++,input[2]++)
    {
        for (input[3] = 0, input[4] = 0, input[5] = 0; input[3] < NUM_HEX_DIGITS; input[3]++, input[4]++, input[5]++,count++)
        {
            ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, input, num_inputs);
            ASSERT_EQ(EXIT_CODE_OK, exit_code);

            ASSERT_EQ(count, set_left_l_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_left_m_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_left_r_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_middle_l_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_middle_m_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_middle_r_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_right_l_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_right_m_hex_digit_fake.call_count);
            ASSERT_EQ(count, set_right_r_hex_digit_fake.call_count);

            ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(true, set_left_m_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(true, set_left_r_hex_digit_fake.arg0_history[count - 1].enabled);

            ASSERT_EQ(true, set_middle_l_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(true, set_middle_m_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(true, set_middle_r_hex_digit_fake.arg0_history[count - 1].enabled);

            ASSERT_EQ(false, set_right_l_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(false, set_right_m_hex_digit_fake.arg0_history[count - 1].enabled);
            ASSERT_EQ(false, set_right_r_hex_digit_fake.arg0_history[count - 1].enabled);

            // The value for the right digit is not asserted because we are only setting two
            ASSERT_EQ(input[0], set_left_l_hex_digit_fake.arg0_history[count - 1].value);
            ASSERT_EQ(input[1], set_left_m_hex_digit_fake.arg0_history[count - 1].value);
            ASSERT_EQ(input[2], set_left_r_hex_digit_fake.arg0_history[count - 1].value);

            ASSERT_EQ(input[3], set_middle_l_hex_digit_fake.arg0_history[count - 1].value);
            ASSERT_EQ(input[4], set_middle_m_hex_digit_fake.arg0_history[count - 1].value);
            ASSERT_EQ(input[5], set_middle_r_hex_digit_fake.arg0_history[count - 1].value);
        }
    }
}

TEST_F(SevenSegDisplaysTest, set_three_hexadecimal_groups)
{
    // Counter used as index for the argument history and call count of fake
    // functions
    size_t count = 1;

    constexpr size_t num_inputs        = 9;
    uint8_t          input[num_inputs] = { 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0};

    for (input[0] = 0, input[1] = 0, input[2] = 0; input[0] < NUM_HEX_DIGITS; input[0]++, input[1]++, input[2]++)
    {
        for (input[3] = 0, input[4] = 0, input[5] = 0; input[3] < NUM_HEX_DIGITS; input[3]++, input[4]++,input[5]++)
        {
            for (input[6] = 0, input[7] = 0, input[8] = 0; input[6] < NUM_HEX_DIGITS; input[6]++, input[7]++,input[8]++, count++)
            {
                ExitCode exit_code = App_SevenSegDisplays_SetHexDigits(seven_seg_displays, input, num_inputs);
                ASSERT_EQ(EXIT_CODE_OK, exit_code);

                ASSERT_EQ(count, set_left_l_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_left_m_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_left_r_hex_digit_fake.call_count);

                ASSERT_EQ(count, set_middle_l_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_middle_m_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_middle_r_hex_digit_fake.call_count);

                ASSERT_EQ(count, set_right_l_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_right_m_hex_digit_fake.call_count);
                ASSERT_EQ(count, set_right_r_hex_digit_fake.call_count);

                ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(true, set_left_m_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(true, set_left_r_hex_digit_fake.arg0_history[count - 1].enabled);

                ASSERT_EQ(true, set_middle_l_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(true, set_middle_m_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(true, set_middle_r_hex_digit_fake.arg0_history[count - 1].enabled);

                ASSERT_EQ(true, set_right_l_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(true, set_right_m_hex_digit_fake.arg0_history[count - 1].enabled);
                ASSERT_EQ(true, set_right_r_hex_digit_fake.arg0_history[count - 1].enabled);


                ASSERT_EQ(input[0], set_left_l_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(input[1], set_left_m_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(input[2], set_left_r_hex_digit_fake.arg0_history[count - 1].value);

                ASSERT_EQ(input[3], set_middle_l_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(input[4], set_middle_m_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(input[5], set_middle_r_hex_digit_fake.arg0_history[count - 1].value);

                ASSERT_EQ(input[6], set_right_l_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(input[7], set_right_m_hex_digit_fake.arg0_history[count - 1].value);
                ASSERT_EQ(input[8], set_right_r_hex_digit_fake.arg0_history[count - 1].value);


                // Sanity check to make sure the argument history is long enough
                ASSERT_EQ(0, set_left_l_hex_digit_fake.arg_histories_dropped);
                ASSERT_EQ(0, set_middle_l_hex_digit_fake.arg_histories_dropped);
                ASSERT_EQ(0, set_right_l_hex_digit_fake.arg_histories_dropped);
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
        ASSERT_EQ(0, set_left_l_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_left_m_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_left_r_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_middle_l_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_middle_m_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_middle_r_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_right_l_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_right_m_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_right_r_hex_digit_fake.call_count);
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
        ASSERT_EQ(0, set_left_l_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_middle_l_hex_digit_fake.call_count);
        ASSERT_EQ(0, set_right_l_hex_digit_fake.call_count);
    }
}

//TEST_F(SevenSegDisplaysTest, set_valid_unsigned_base10_values)
//{
//    App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 0);
//    ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[0].enabled);
//    ASSERT_EQ(false, set_middle_l_hex_digit_fake.arg0_history[0].enabled);
//    ASSERT_EQ(false, set_right_l_hex_digit_fake.arg0_history[0].enabled);
//    ASSERT_EQ(0, set_left_l_hex_digit_fake.arg0_history[0].value);
//
//    App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 500);
//    ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[1].enabled);
//    ASSERT_EQ(true, set_middle_l_hex_digit_fake.arg0_history[1].enabled);
//    ASSERT_EQ(true, set_right_l_hex_digit_fake.arg0_history[1].enabled);
//    ASSERT_EQ(0, set_left_l_hex_digit_fake.arg0_history[1].value);
//    ASSERT_EQ(0, set_middle_l_hex_digit_fake.arg0_history[1].value);
//    ASSERT_EQ(5, set_right_l_hex_digit_fake.arg0_history[1].value);
//
//    App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 999);
//    ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[2].enabled);
//    ASSERT_EQ(true, set_middle_l_hex_digit_fake.arg0_history[2].enabled);
//    ASSERT_EQ(true, set_right_l_hex_digit_fake.arg0_history[2].enabled);
//    ASSERT_EQ(9, set_left_l_hex_digit_fake.arg0_history[2].value);
//    ASSERT_EQ(9, set_middle_l_hex_digit_fake.arg0_history[2].value);
//    ASSERT_EQ(9, set_right_l_hex_digit_fake.arg0_history[2].value);
//}

TEST_F(SevenSegDisplaysTest, set_invalid_unsigned_base10_values)
{
    ExitCode exit_code = EXIT_CODE_OK;
    exit_code          = App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 1000000000);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code);

    ASSERT_EQ(0, set_left_l_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_left_m_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_left_r_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_middle_l_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_middle_m_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_middle_r_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_right_l_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_right_m_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_right_r_hex_digit_fake.call_count);
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
    ExitCode exit_code = App_SevenSegDisplays_SetUnsignedBase10Value(seven_seg_displays, 1000000000);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code);
    ASSERT_EQ(0, display_value_callback_fake.call_count);
}
