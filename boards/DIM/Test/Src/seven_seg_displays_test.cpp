#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SevenSegDisplays.h"

    DEFINE_FFF_GLOBALS;
    FAKE_VALUE_FUNC(float, get_state_of_charge);
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
            get_state_of_charge, set_left_hex_digit, set_middle_hex_digit,
            set_right_hex_digit);

        RESET_FAKE(get_state_of_charge);
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

TEST_F(SevenSegDisplaysTest, write_max_state_of_chrage)
{
    get_state_of_charge_fake.return_val = 100.0f;

    App_SevenSegDisplays_WriteStateOfCharge(seven_segment_displays);

    ASSERT_EQ(1, set_left_hex_digit_fake.arg0_history[0]);
    ASSERT_EQ(0, set_middle_hex_digit_fake.arg0_history[0]);
    ASSERT_EQ(0, set_right_hex_digit_fake.arg0_history[0]);

    ASSERT_EQ(1, get_state_of_charge_fake.call_count);
    ASSERT_EQ(1, set_left_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_middle_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_right_hex_digit_fake.call_count);
}

TEST_F(SevenSegDisplaysTest, write_min_state_of_chrage)
{
    get_state_of_charge_fake.return_val = 0.0f;

    App_SevenSegDisplays_WriteStateOfCharge(seven_segment_displays);

    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[0]);
    ASSERT_EQ(0, set_middle_hex_digit_fake.arg0_history[0]);
    ASSERT_EQ(0, set_right_hex_digit_fake.arg0_history[0]);

    ASSERT_EQ(1, get_state_of_charge_fake.call_count);
    ASSERT_EQ(1, set_left_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_middle_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_right_hex_digit_fake.call_count);
}

TEST_F(SevenSegDisplaysTest, write_state_of_chrage_with_decimals)
{
    get_state_of_charge_fake.return_val = 23.456789f;

    App_SevenSegDisplays_WriteStateOfCharge(seven_segment_displays);

    ASSERT_EQ(0, set_left_hex_digit_fake.arg0_history[0]);
    ASSERT_EQ(2, set_middle_hex_digit_fake.arg0_history[0]);
    ASSERT_EQ(3, set_right_hex_digit_fake.arg0_history[0]);

    ASSERT_EQ(1, get_state_of_charge_fake.call_count);
    ASSERT_EQ(1, set_left_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_middle_hex_digit_fake.call_count);
    ASSERT_EQ(1, set_right_hex_digit_fake.call_count);
}

TEST_F(SevenSegDisplaysTest, write_overflow_state_of_charge)
{
    get_state_of_charge_fake.return_val = 101.0f;

    ASSERT_DEATH(
        { App_SevenSegDisplays_WriteStateOfCharge(seven_segment_displays); },
        "");
}

TEST_F(SevenSegDisplaysTest, write_underflow_state_of_charge)
{
    get_state_of_charge_fake.return_val = -1.0f;

    ASSERT_DEATH(
        { App_SevenSegDisplays_WriteStateOfCharge(seven_segment_displays); },
        "");
}
