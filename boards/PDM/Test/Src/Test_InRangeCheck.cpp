#include <math.h>
#include "Test_Pdm.h"

extern "C"
{
#include "App_InRangeCheck.h"
}

FAKE_VALUE_FUNC(float, get_value);

class InRangeCheckTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        in_range_check = App_InRangeCheck_Create(
            get_value, DEFAULT_MIN_VALUE, DEFAULT_MAX_VAVLUE);

        RESET_FAKE(get_value);

        FFF_RESET_HISTORY();
    }

    void TearDown() override
    {
        ASSERT_TRUE(in_range_check != NULL);
        App_InRangeCheck_Destroy(in_range_check);
        in_range_check = NULL;
    }

    struct InRangeCheck *in_range_check;

    const float DEFAULT_MIN_VALUE  = 5.0f;
    const float DEFAULT_MAX_VAVLUE = 6.0f;
};

TEST_F(InRangeCheckTest, value_in_range)
{
    float value;

    get_value_fake.return_val = (DEFAULT_MIN_VALUE + DEFAULT_MAX_VAVLUE) / 2.0f;
    ASSERT_EQ(
        VALUE_IN_RANGE, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);
}

TEST_F(InRangeCheckTest, value_underflow)
{
    float value;

    // The range is inclusive, so the minimum value should not trigger an error
    get_value_fake.return_val = DEFAULT_MIN_VALUE;
    ASSERT_EQ(
        VALUE_IN_RANGE, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);

    get_value_fake.return_val =
        std::nextafter(DEFAULT_MIN_VALUE, std::numeric_limits<float>::min());
    ASSERT_EQ(
        VALUE_UNDERFLOW, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);
}

TEST_F(InRangeCheckTest, value_overflow)
{
    float value;

    // The range is inclusive, so the maximum value should not trigger an error
    get_value_fake.return_val = DEFAULT_MAX_VAVLUE;
    ASSERT_EQ(
        VALUE_IN_RANGE, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);

    get_value_fake.return_val =
        std::nextafter(DEFAULT_MAX_VAVLUE, std::numeric_limits<float>::max());
    ASSERT_EQ(
        VALUE_OVERFLOW, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);
}
