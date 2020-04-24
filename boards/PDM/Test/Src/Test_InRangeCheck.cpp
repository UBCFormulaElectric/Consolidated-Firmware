#include <math.h>
#include "Test_InRangeCheck.h"

extern "C"
{
#include "App_InRangeCheck.h"
}

FAKE_VALUE_FUNC(float, get_value);
FAKE_VOID_FUNC(error_callback, struct InRangeCheck *);

class InRangeCheckTest : public PdmTest
{
  protected:
    virtual void SetUp()
    {
        voltage_check = App_InRangeCheck_Create(
            get_value, DEFAULT_MIN_VALUE, DEFAULT_MAX_VAVLUE, error_callback);

        RESET_FAKE(get_value);
        RESET_FAKE(error_callback);

        FFF_RESET_HISTORY();
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(voltage_check != NULL);
        App_InRangeCheck_Destroy(voltage_check);
        voltage_check = NULL;
    }

    struct InRangeCheck *voltage_check;

    const float DEFAULT_MIN_VALUE  = 5.0f;
    const float DEFAULT_MAX_VAVLUE = 6.0f;
};

TEST_F(InRangeCheckTest, value_in_range)
{
    get_value_fake.return_val = (DEFAULT_MIN_VALUE + DEFAULT_MAX_VAVLUE) / 2.0f;
    App_InRangeCheck_Tick(voltage_check);
    ASSERT_EQ(App_InRangeCheck_GetStatus(voltage_check), VALUE_IN_RANGE);
}

TEST_F(InRangeCheckTest, value_undervalue)
{
    get_value_fake.return_val = DEFAULT_MIN_VALUE;
    App_InRangeCheck_Tick(voltage_check);
    ASSERT_EQ(App_InRangeCheck_GetStatus(voltage_check), VALUE_IN_RANGE);
    ASSERT_EQ(error_callback_fake.call_count, 0);

    get_value_fake.return_val =
        std::nextafter(DEFAULT_MIN_VALUE, std::numeric_limits<float>::min());
    App_InRangeCheck_Tick(voltage_check);
    ASSERT_EQ(App_InRangeCheck_GetStatus(voltage_check), VALUE_UNDERFLOW);
    ASSERT_EQ(error_callback_fake.call_count, 1);
}

TEST_F(InRangeCheckTest, value_overflow)
{
    get_value_fake.return_val = DEFAULT_MAX_VAVLUE;
    App_InRangeCheck_Tick(voltage_check);
    ASSERT_EQ(App_InRangeCheck_GetStatus(voltage_check), VALUE_IN_RANGE);
    ASSERT_EQ(error_callback_fake.call_count, 0);

    get_value_fake.return_val =
        std::nextafter(DEFAULT_MAX_VAVLUE, std::numeric_limits<float>::max());
    App_InRangeCheck_Tick(voltage_check);
    ASSERT_EQ(App_InRangeCheck_GetStatus(voltage_check), VALUE_OVERFLOW);
    ASSERT_EQ(error_callback_fake.call_count, 1);
}
