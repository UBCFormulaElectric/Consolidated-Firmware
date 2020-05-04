#include <math.h>
#include "Test_InRangeCheck.h"

extern "C"
{
#include "App_InRangeCheck.h"
}

FAKE_VALUE_FUNC(float, get_value);

void InRangeCheckTest::TearDownInRangeCheck(
    struct InRangeCheck *&in_range_check_to_teardown)
{
    ASSERT_TRUE(in_range_check_to_teardown != NULL);
    App_InRangeCheck_Destroy(in_range_check_to_teardown);
    in_range_check_to_teardown = NULL;
}

void InRangeCheckTest::SetUp()
{
    in_range_check = App_InRangeCheck_Create(
        get_value, DEFAULT_IN_RANGE_CHECK_MIN_VALUE,
        DEFAULT_IN_RANGE_CHECK_MAX_VALUE);

    RESET_FAKE(get_value);
}

void InRangeCheckTest::TearDown()
{
    ASSERT_TRUE(in_range_check != NULL);
    App_InRangeCheck_Destroy(in_range_check);
    in_range_check = NULL;
}

TEST_F(InRangeCheckTest, value_in_range)
{
    float value;

    get_value_fake.return_val =
        (DEFAULT_IN_RANGE_CHECK_MIN_VALUE + DEFAULT_IN_RANGE_CHECK_MAX_VALUE) /
        2.0f;
    ASSERT_EQ(
        VALUE_IN_RANGE, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);
}

TEST_F(InRangeCheckTest, value_underflow)
{
    float value;

    // The range is inclusive, so the minimum value should not trigger an error
    get_value_fake.return_val = DEFAULT_IN_RANGE_CHECK_MIN_VALUE;
    ASSERT_EQ(
        VALUE_IN_RANGE, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);

    get_value_fake.return_val = std::nextafter(
        DEFAULT_IN_RANGE_CHECK_MIN_VALUE, std::numeric_limits<float>::min());
    ASSERT_EQ(
        VALUE_UNDERFLOW, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);
}

TEST_F(InRangeCheckTest, value_overflow)
{
    float value;

    // The range is inclusive, so the maximum value should not trigger an error
    get_value_fake.return_val = DEFAULT_IN_RANGE_CHECK_MAX_VALUE;
    ASSERT_EQ(
        VALUE_IN_RANGE, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);

    get_value_fake.return_val = std::nextafter(
        DEFAULT_IN_RANGE_CHECK_MAX_VALUE, std::numeric_limits<float>::max());
    ASSERT_EQ(
        VALUE_OVERFLOW, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);
}
