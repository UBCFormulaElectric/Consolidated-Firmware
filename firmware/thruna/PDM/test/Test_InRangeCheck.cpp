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
        in_range_check =
            App_InRangeCheck_Create(get_value, DEFAULT_IN_RANGE_CHECK_MIN_VALUE, DEFAULT_IN_RANGE_CHECK_MAX_VALUE);

        RESET_FAKE(get_value);
    }

    void TearDown() override { TearDownObject(in_range_check, App_InRangeCheck_Destroy); }

    struct InRangeCheck *in_range_check;

    const float DEFAULT_IN_RANGE_CHECK_MIN_VALUE = 5.0f;
    const float DEFAULT_IN_RANGE_CHECK_MAX_VALUE = 6.0f;
};

TEST_F(InRangeCheckTest, value_in_range)
{
    float value;

    get_value_fake.return_val = (DEFAULT_IN_RANGE_CHECK_MIN_VALUE + DEFAULT_IN_RANGE_CHECK_MAX_VALUE) / 2.0f;
    ASSERT_EQ(VALUE_IN_RANGE, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);
}

TEST_F(InRangeCheckTest, value_underflow)
{
    float value;

    // The range is inclusive, so the minimum value should not trigger an error
    get_value_fake.return_val = DEFAULT_IN_RANGE_CHECK_MIN_VALUE;
    ASSERT_EQ(VALUE_IN_RANGE, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);

    get_value_fake.return_val = std::nextafter(DEFAULT_IN_RANGE_CHECK_MIN_VALUE, std::numeric_limits<float>::min());
    ASSERT_EQ(VALUE_UNDERFLOW, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);
}

TEST_F(InRangeCheckTest, value_overflow)
{
    float value;

    // The range is inclusive, so the maximum value should not trigger an error
    get_value_fake.return_val = DEFAULT_IN_RANGE_CHECK_MAX_VALUE;
    ASSERT_EQ(VALUE_IN_RANGE, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);

    get_value_fake.return_val = std::nextafter(DEFAULT_IN_RANGE_CHECK_MAX_VALUE, std::numeric_limits<float>::max());
    ASSERT_EQ(VALUE_OVERFLOW, App_InRangeCheck_GetValue(in_range_check, &value));
    ASSERT_EQ(get_value_fake.return_val, value);
}

TEST_F(InRangeCheckTest, invalid_min_and_max)
{
#ifdef NDEBUG
    // Using ASSERT_DEBUG_DEATH with a function that has malloc in it (i.e.
    // App_InRangeCheck_Create) causes Valgrind to think there's memory leak in
    // Release Mode but not Debug Mode. To get around this, we're just going to
    // disable this test in Release Mode for the time being until we find a
    // better workaround.
#else
    // It doesn't make sense to perform a range check if the min_value is
    // greater than the max_value.
    const float max_value = 1.0f;
    const float min_value = std::nextafter(max_value, std::numeric_limits<float>::max());

    ASSERT_DEBUG_DEATH(App_InRangeCheck_Create(get_value, min_value, max_value), "");
#endif
}
