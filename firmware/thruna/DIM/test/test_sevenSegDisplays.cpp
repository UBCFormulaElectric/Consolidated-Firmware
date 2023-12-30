#include <gtest/gtest.h>
#include <math.h>

#include "fake_io_sevenSegDisplays.hpp"

extern "C"
{
#include "app_sevenSegDisplays.h"
}

class SevenSegDisplaysTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        app_sevenSegDisplays_init();

        // Reset fakes.
        fake_io_sevenSegDisplays_disable_reset();
        fake_io_sevenSegDisplays_setValue_reset();
        fake_io_sevenSegDisplays_writeCommands_reset();
    }

    const SevenSegsConfig seven_segs_config = {};
};

TEST_F(SevenSegDisplaysTest, set_left_hexadecimal_group)
{
    // Ensuring that the SetGroup function only touches the commanded group of displays (left).

    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, 111));

    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(3, fake_io_sevenSegDisplays_setValue_callCount());
    ASSERT_EQ(1, fake_io_sevenSegDisplays_writeCommands_callCount());
}

TEST_F(SevenSegDisplaysTest, set_middle_hexadecimal_group)
{
    // Ensuring that the SetGroup function only touches the commanded group of displays (middle).

    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_M, 111));

    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(3, fake_io_sevenSegDisplays_setValue_callCount());
    ASSERT_EQ(1, fake_io_sevenSegDisplays_writeCommands_callCount());
}

TEST_F(SevenSegDisplaysTest, set_right_hexadecimal_group)
{
    // Ensuring that the SetGroup function only touches the commanded group of displays (middle).

    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_R, 111));

    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(3, fake_io_sevenSegDisplays_setValue_callCount());
    ASSERT_EQ(1, fake_io_sevenSegDisplays_writeCommands_callCount());
}

TEST_F(SevenSegDisplaysTest, set_overflow_to_each_group)
{
    uint32_t overflow_val = pow(10, (int)NUM_SEVEN_SEG_SUBPOSITIONS);

    ASSERT_EQ(false, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, overflow_val));
    ASSERT_EQ(false, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_M, overflow_val));
    ASSERT_EQ(false, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_R, overflow_val));
}

TEST_F(SevenSegDisplaysTest, set_right_hexadecimal_group_negative_value)
{
    // Ensuring that the SetGroup function only touches the commanded group of displays (middle).
    // Ensuring the given value can be negative value

    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_R, -111));

    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        0, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_1, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_1, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_1, true));

    ASSERT_EQ(3, fake_io_sevenSegDisplays_setValue_callCount());
    ASSERT_EQ(1, fake_io_sevenSegDisplays_writeCommands_callCount());
}

TEST_F(SevenSegDisplaysTest, set_all_zeros)
{
    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, 0));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_0, true));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_0, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_0, false));

    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_M, 0));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_0, true));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_0, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_0, false));

    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_R, 0));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_0, true));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_0, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_0, false));
}

TEST_F(SevenSegDisplaysTest, set_valid_decimal_values)
{
    // Check various decimal values on all 3 displays.
    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, 999));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_9, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_9, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_L, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_9, true));

    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_M, 95.3));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_9, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_5, true));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_M, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_3, false));

    ASSERT_EQ(true, app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_R, 0.60));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_L, HEX_DIGIT_0, true));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_M, HEX_DIGIT_6, false));
    ASSERT_EQ(
        1, fake_io_sevenSegDisplays_setValue_callCountForArgs(
               SEVEN_SEG_GROUP_R, SEVEN_SEG_SUBPOSITION_R, HEX_DIGIT_0, false));
}
