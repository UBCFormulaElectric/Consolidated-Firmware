#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SevenSegDisplay.h"
#include "App_SevenSegDisplays.h"

    DEFINE_FFF_GLOBALS;
    FAKE_VALUE_FUNC(float, get_state_of_charge);
    FAKE_VOID_FUNC(set_right_digit, enum SevenSegDisplay_Digit);
    FAKE_VOID_FUNC(set_right_letter, enum SevenSegDisplay_Letter);
    FAKE_VOID_FUNC(set_middle_digit, enum SevenSegDisplay_Digit);
    FAKE_VOID_FUNC(set_middle_letter, enum SevenSegDisplay_Letter);
    FAKE_VOID_FUNC(set_left_digit, enum SevenSegDisplay_Digit);
    FAKE_VOID_FUNC(set_left_letter, enum SevenSegDisplay_Letter);
}

struct SevenSegDisplay *App_SevenSegDisplay_Create(
    void (*const set_digit)(enum SevenSegDisplay_Digit digit),
    void (*const set_letter)(enum SevenSegDisplay_Letter letter));
class SevenSegDisplaysTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        right_display =
            App_SevenSegDisplay_Create(set_right_digit, set_right_letter);
        middle_display =
            App_SevenSegDisplay_Create(set_middle_digit, set_middle_letter);
        left_display =
            App_SevenSegDisplay_Create(set_left_digit, set_left_letter);

        seven_segment_displays = App_SevenSegDisplays_Create(
            get_state_of_charge, left_display, middle_display, right_display);

        RESET_FAKE(get_state_of_charge);
        RESET_FAKE(set_right_digit);
        RESET_FAKE(set_right_letter);
        RESET_FAKE(set_middle_digit);
        RESET_FAKE(set_middle_letter);
        RESET_FAKE(set_left_digit);
        RESET_FAKE(set_left_letter);

        FFF_RESET_HISTORY();
    }
    void TearDown() override
    {
        App_SevenSegDisplay_Destroy(right_display);
        App_SevenSegDisplay_Destroy(middle_display);
        App_SevenSegDisplay_Destroy(left_display);
        App_SevenSegDisplays_Destroy(seven_segment_displays);
    }

    struct SevenSegDisplay * left_display;
    struct SevenSegDisplay * middle_display;
    struct SevenSegDisplay * right_display;
    struct SevenSegDisplays *seven_segment_displays;
};

TEST_F(SevenSegDisplaysTest, write_max_state_of_chrage)
{
    get_state_of_charge_fake.return_val = 100.0f;

    App_SevenSegDisplays_WriteStateOfCharge(seven_segment_displays);

    ASSERT_EQ(1, set_left_digit_fake.arg0_history[0]);
    ASSERT_EQ(0, set_middle_digit_fake.arg0_history[0]);
    ASSERT_EQ(0, set_right_digit_fake.arg0_history[0]);

    ASSERT_EQ(1, get_state_of_charge_fake.call_count);
    ASSERT_EQ(1, set_left_digit_fake.call_count);
    ASSERT_EQ(1, set_middle_digit_fake.call_count);
    ASSERT_EQ(1, set_right_digit_fake.call_count);

    ASSERT_EQ(fff.call_history[0], (void *)get_state_of_charge);
    ASSERT_EQ(fff.call_history[1], (void *)set_left_digit);
    ASSERT_EQ(fff.call_history[2], (void *)set_middle_digit);
    ASSERT_EQ(fff.call_history[3], (void *)set_right_digit);
}

TEST_F(SevenSegDisplaysTest, write_min_state_of_chrage)
{
    get_state_of_charge_fake.return_val = 0.0f;

    App_SevenSegDisplays_WriteStateOfCharge(seven_segment_displays);

    ASSERT_EQ(0, set_left_digit_fake.arg0_history[0]);
    ASSERT_EQ(0, set_middle_digit_fake.arg0_history[0]);
    ASSERT_EQ(0, set_right_digit_fake.arg0_history[0]);

    ASSERT_EQ(1, get_state_of_charge_fake.call_count);
    ASSERT_EQ(1, set_left_digit_fake.call_count);
    ASSERT_EQ(1, set_middle_digit_fake.call_count);
    ASSERT_EQ(1, set_right_digit_fake.call_count);

    ASSERT_EQ(fff.call_history[0], (void *)get_state_of_charge);
    ASSERT_EQ(fff.call_history[1], (void *)set_left_digit);
    ASSERT_EQ(fff.call_history[2], (void *)set_middle_digit);
    ASSERT_EQ(fff.call_history[3], (void *)set_right_digit);
}

TEST_F(SevenSegDisplaysTest, write_state_of_chrage_with_decimals)
{
    get_state_of_charge_fake.return_val = 23.456789f;

    App_SevenSegDisplays_WriteStateOfCharge(seven_segment_displays);

    ASSERT_EQ(0, set_left_digit_fake.arg0_history[0]);
    ASSERT_EQ(2, set_middle_digit_fake.arg0_history[0]);
    ASSERT_EQ(3, set_right_digit_fake.arg0_history[0]);

    ASSERT_EQ(1, get_state_of_charge_fake.call_count);
    ASSERT_EQ(1, set_left_digit_fake.call_count);
    ASSERT_EQ(1, set_middle_digit_fake.call_count);
    ASSERT_EQ(1, set_right_digit_fake.call_count);

    ASSERT_EQ(fff.call_history[0], (void *)get_state_of_charge);
    ASSERT_EQ(fff.call_history[1], (void *)set_left_digit);
    ASSERT_EQ(fff.call_history[2], (void *)set_middle_digit);
    ASSERT_EQ(fff.call_history[3], (void *)set_right_digit);
}

TEST_F(SevenSegDisplaysTest, write_underflow_state_of_charge)
{
    get_state_of_charge_fake.return_val = 101.f;

    ASSERT_DEATH(
        {App_SevenSegDisplays_WriteStateOfCharge(seven_segment_displays);},
        "");
}