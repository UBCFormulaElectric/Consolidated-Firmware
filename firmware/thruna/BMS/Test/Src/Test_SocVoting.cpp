#include <math.h>
#include "Test_Bms.h"

extern "C"
{
#include "App_Soc.h"
}

class SocVotingTest : public testing::Test
{
  protected:
    float       result;
    const float TEST_VALUE          = 0.5f;
    const float TEST_MAX_DIFFERENCE = 1.0f;
    const float UNDERFLOW_SOC       = std::nextafter(0.0f, std::numeric_limits<float>::lowest());
    const float OVERFLOW_SOC        = std::nextafter(100.0f, std::numeric_limits<float>::max());
};

TEST_F(SocVotingTest, max_difference_underflow)
{
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(UNDERFLOW_SOC, 0.0f, 0.0f, 0.0f, &result));
}

TEST_F(SocVotingTest, one_number_underflow)
{
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, UNDERFLOW_SOC, 0.0f, 0.0f, &result));
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, 0.0f, UNDERFLOW_SOC, 0.0f, &result));
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, 0.0f, 0.0f, UNDERFLOW_SOC, &result));
}

TEST_F(SocVotingTest, one_number_overflow)
{
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, OVERFLOW_SOC, 0.0f, 0.0f, &result));
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, 0.0f, OVERFLOW_SOC, 0.0f, &result));
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, 0.0f, 0.0f, OVERFLOW_SOC, &result));
}

TEST_F(SocVotingTest, two_numbers_underflow)
{
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, UNDERFLOW_SOC, UNDERFLOW_SOC, 0.0f, &result));
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, 0.0f, UNDERFLOW_SOC, UNDERFLOW_SOC, &result));
}

TEST_F(SocVotingTest, two_numbers_overflow)
{
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, OVERFLOW_SOC, OVERFLOW_SOC, 0.0f, &result));
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, 0.0f, OVERFLOW_SOC, OVERFLOW_SOC, &result));
}

TEST_F(SocVotingTest, three_numbers_underflow)
{
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, UNDERFLOW_SOC, UNDERFLOW_SOC, UNDERFLOW_SOC, &result));
}

TEST_F(SocVotingTest, three_numbers_overflow)
{
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, App_Soc_Vote(0.0f, OVERFLOW_SOC, OVERFLOW_SOC, OVERFLOW_SOC, &result));
}

TEST_F(SocVotingTest, three_identical_numbers)
{
    ASSERT_EQ(EXIT_CODE_OK, App_Soc_Vote(0.0f, TEST_VALUE, TEST_VALUE, TEST_VALUE, &result));
    ASSERT_EQ(TEST_VALUE, result);
}

TEST_F(SocVotingTest, difference_is_smaller_than_max_difference)
{
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_Soc_Vote(
            std::nextafter(TEST_VALUE, std::numeric_limits<float>::max()), 0.0f, TEST_VALUE, 2 * TEST_VALUE, &result));
    ASSERT_EQ(TEST_VALUE / 2.0f, result);
}

TEST_F(SocVotingTest, difference_is_equal_to_max_difference)
{
    ASSERT_EQ(EXIT_CODE_OK, App_Soc_Vote(TEST_VALUE, 0.0f, TEST_VALUE, 2 * TEST_VALUE, &result));
    ASSERT_EQ(TEST_VALUE / 2.0f, result);
}

TEST_F(SocVotingTest, difference_is_larger_than_max_difference)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_Soc_Vote(
                          std::nextafter(TEST_VALUE, std::numeric_limits<float>::lowest()), 0.0f, TEST_VALUE,
                          2 * TEST_VALUE, &result));
    ASSERT_TRUE(isnan(result));
}
