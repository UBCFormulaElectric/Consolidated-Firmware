#include "Test_Dim.h"

extern "C"
{
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
}

FAKE_VOID_FUNC(set_right_l_hex_digit, struct SevenSegHexDigit, int);
FAKE_VOID_FUNC(set_right_m_hex_digit, struct SevenSegHexDigit, int);
FAKE_VOID_FUNC(set_right_r_hex_digit, struct SevenSegHexDigit, int);
FAKE_VOID_FUNC(set_middle_l_hex_digit, struct SevenSegHexDigit, int);
FAKE_VOID_FUNC(set_middle_m_hex_digit, struct SevenSegHexDigit, int);
FAKE_VOID_FUNC(set_middle_r_hex_digit, struct SevenSegHexDigit, int);
FAKE_VOID_FUNC(set_left_l_hex_digit, struct SevenSegHexDigit, int);
FAKE_VOID_FUNC(set_left_m_hex_digit, struct SevenSegHexDigit, int);
FAKE_VOID_FUNC(set_left_r_hex_digit, struct SevenSegHexDigit, int);
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
            left_l_seven_seg_display, left_m_seven_seg_display, left_r_seven_seg_display, middle_l_seven_seg_display,
            middle_m_seven_seg_display, middle_r_seven_seg_display, right_l_seven_seg_display,
            right_m_seven_seg_display, right_r_seven_seg_display, display_value_callback);

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

TEST_F(SevenSegDisplaysTest, set_left_hexadecimal_group)
{
    // Ensuring that the SetGroupL function only touches the left group of displays

    ExitCode exit_code = App_SevenSegDisplays_SetGroupL(seven_seg_displays, 100);
    ASSERT_EQ(EXIT_CODE_OK, exit_code);

    ASSERT_EQ(1, set_left_l_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_left_m_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_left_r_hex_digit_fake.call_count);

    ASSERT_EQ(0, set_middle_l_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_middle_m_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_middle_r_hex_digit_fake.call_count);

    ASSERT_EQ(0, set_right_l_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_right_m_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_right_r_hex_digit_fake.call_count);

    ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_left_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_left_r_hex_digit_fake.arg0_history[0].enabled);

    ASSERT_EQ(false, set_middle_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_middle_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_middle_r_hex_digit_fake.arg0_history[0].enabled);

    ASSERT_EQ(false, set_right_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_right_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_right_r_hex_digit_fake.arg0_history[0].enabled);
}

TEST_F(SevenSegDisplaysTest, set_middle_hexadecimal_group)
{
    // ensuring that the SetGroupM function only touches the middle group of displays

    ExitCode exit_code = App_SevenSegDisplays_SetGroupM(seven_seg_displays, 100);
    ASSERT_EQ(EXIT_CODE_OK, exit_code);

    ASSERT_EQ(0, set_left_l_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_left_m_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_left_r_hex_digit_fake.call_count);

    ASSERT_EQ(1, set_middle_l_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_middle_m_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_middle_r_hex_digit_fake.call_count);

    ASSERT_EQ(0, set_right_l_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_right_m_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_right_r_hex_digit_fake.call_count);

    ASSERT_EQ(false, set_left_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_left_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_left_r_hex_digit_fake.arg0_history[0].enabled);

    ASSERT_EQ(true, set_middle_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_middle_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_middle_r_hex_digit_fake.arg0_history[0].enabled);

    ASSERT_EQ(false, set_right_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_right_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_right_r_hex_digit_fake.arg0_history[0].enabled);
}

TEST_F(SevenSegDisplaysTest, set_right_hexadecimal_group)
{
    // ensuring that the SetGroupR function only touches the right group of displays

    ExitCode exit_code = App_SevenSegDisplays_SetGroupR(seven_seg_displays, 100);
    ASSERT_EQ(EXIT_CODE_OK, exit_code);

    ASSERT_EQ(0, set_left_l_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_left_m_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_left_r_hex_digit_fake.call_count);

    ASSERT_EQ(0, set_middle_l_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_middle_m_hex_digit_fake.call_count);
    ASSERT_EQ(0, set_middle_r_hex_digit_fake.call_count);

    ASSERT_EQ(1, set_right_l_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_right_m_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_right_r_hex_digit_fake.call_count);

    ASSERT_EQ(false, set_left_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_left_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_left_r_hex_digit_fake.arg0_history[0].enabled);

    ASSERT_EQ(false, set_middle_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_middle_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(false, set_middle_r_hex_digit_fake.arg0_history[0].enabled);

    ASSERT_EQ(true, set_right_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_right_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_right_r_hex_digit_fake.arg0_history[0].enabled);
}

TEST_F(SevenSegDisplaysTest, set_overflow_to_each_group)
{
    uint32_t over_flow_val = pow(10, NUM_IN_GROUP);

    ExitCode exit_code_L = App_SevenSegDisplays_SetGroupL(seven_seg_displays, over_flow_val);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code_L);

    ExitCode exit_code_M = App_SevenSegDisplays_SetGroupM(seven_seg_displays, over_flow_val);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code_M);

    ExitCode exit_code_R = App_SevenSegDisplays_SetGroupR(seven_seg_displays, over_flow_val);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code_R);
}

TEST_F(SevenSegDisplaysTest, set_valid_unsigned_base10_values)
{
    ExitCode exit_code = App_SevenSegDisplays_SetGroupL(seven_seg_displays, 0);
    ASSERT_EQ(EXIT_CODE_OK, exit_code);
    ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_left_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_left_r_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(0, set_left_r_hex_digit_fake.arg0_history[0].value);
    ASSERT_EQ(0, set_left_m_hex_digit_fake.arg0_history[0].value);
    ASSERT_EQ(10, set_left_l_hex_digit_fake.arg0_history[0].value); //the 10 indicates the placeholder value to indicate a decimal point

    App_SevenSegDisplays_SetGroupM(seven_seg_displays, 0);
    ASSERT_EQ(true, set_middle_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_middle_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_middle_r_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(10, set_middle_l_hex_digit_fake.arg0_history[0].value);

    App_SevenSegDisplays_SetGroupR(seven_seg_displays, 0);
    ASSERT_EQ(true, set_right_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_right_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_right_r_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(10, set_right_l_hex_digit_fake.arg0_history[0].value);

    // checking if we modify one group, the other groups should stay the same

    App_SevenSegDisplays_SetGroupL(seven_seg_displays, 129);
    ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_left_m_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_left_r_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(1, set_left_l_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(2, set_left_m_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(9, set_left_r_hex_digit_fake.arg0_history[1].value);

    ASSERT_EQ(false, set_middle_l_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(false, set_middle_m_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(false, set_middle_r_hex_digit_fake.arg0_history[1].enabled);

    ASSERT_EQ(false, set_right_l_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(false, set_right_m_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(false, set_right_r_hex_digit_fake.arg0_history[1].enabled);

    App_SevenSegDisplays_SetGroupR(seven_seg_displays, 308);

    ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_left_m_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_left_r_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(1, set_left_l_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(2, set_left_m_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(9, set_left_r_hex_digit_fake.arg0_history[1].value);

    ASSERT_EQ(false, set_middle_l_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(false, set_middle_m_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(false, set_middle_r_hex_digit_fake.arg0_history[1].enabled);

    ASSERT_EQ(true, set_right_l_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_right_m_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_right_r_hex_digit_fake.arg0_history[1].enabled);

    ASSERT_EQ(3, set_right_l_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(0, set_right_m_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(8, set_right_r_hex_digit_fake.arg0_history[1].value);

    App_SevenSegDisplays_SetGroupM(seven_seg_displays, 935);
    ASSERT_EQ(true, set_middle_l_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_middle_m_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(true, set_middle_r_hex_digit_fake.arg0_history[1].enabled);
    ASSERT_EQ(9, set_middle_l_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(3, set_middle_m_hex_digit_fake.arg0_history[1].value);
    ASSERT_EQ(5, set_middle_r_hex_digit_fake.arg0_history[1].value);
}

TEST_F(SevenSegDisplaysTest, set_valid_decimal_values)
{
    ExitCode exit_code = App_SevenSegDisplays_SetGroupL(seven_seg_displays, 0.01);
    ASSERT_EQ(EXIT_CODE_OK, exit_code);
    ASSERT_EQ(true, set_left_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_left_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_left_r_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(1, set_left_r_hex_digit_fake.arg0_history[0].value);
    ASSERT_EQ(0, set_left_m_hex_digit_fake.arg0_history[0].value);
    ASSERT_EQ(10, set_left_l_hex_digit_fake.arg0_history[0].value); //the 10 indicates the placeholder value to indicate a decimal point

    App_SevenSegDisplays_SetGroupM(seven_seg_displays, 42.2);
    ASSERT_EQ(true, set_middle_l_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_middle_m_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(true, set_middle_r_hex_digit_fake.arg0_history[0].enabled);
    ASSERT_EQ(2, set_middle_r_hex_digit_fake.arg0_history[0].value);
    ASSERT_EQ(12, set_middle_m_hex_digit_fake.arg0_history[0].value);
    ASSERT_EQ(4, set_middle_l_hex_digit_fake.arg0_history[0].value);

    App_SevenSegDisplays_SetGroupR(seven_seg_displays, .308);
    ASSERT_EQ(10, set_right_l_hex_digit_fake.arg0_history[0].value);
    ASSERT_EQ(3, set_right_m_hex_digit_fake.arg0_history[0].value);
    ASSERT_EQ(0, set_right_r_hex_digit_fake.arg0_history[0].value);
}

TEST_F(SevenSegDisplaysTest, set_invalid_unsigned_base10_value_does_not_invoke_callback_function)
{
    ExitCode exit_code_L = App_SevenSegDisplays_SetGroupL(seven_seg_displays, 100000000);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code_L);
    ASSERT_EQ(0, display_value_callback_fake.call_count);

    ExitCode exit_code_M = App_SevenSegDisplays_SetGroupM(seven_seg_displays, 100000000);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code_M);
    ASSERT_EQ(0, display_value_callback_fake.call_count);

    ExitCode exit_code_R = App_SevenSegDisplays_SetGroupR(seven_seg_displays, 100000000);
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_code_R);
    ASSERT_EQ(0, display_value_callback_fake.call_count);
}
