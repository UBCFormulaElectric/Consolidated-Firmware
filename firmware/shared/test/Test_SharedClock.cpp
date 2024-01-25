#include "Test_Shared.h"

extern "C"
{
#include "App_SharedClock.h"
}

class ClockTest : public testing::Test
{
  protected:
    void SetUp() override { clock = App_SharedClock_Create(); }

    void TearDown() override { TearDownObject(clock, App_SharedClock_Destroy); }

    struct Clock * clock;
};

TEST_F(ClockTest, get_current_time_at_zero_second)
{
    App_SharedClock_SetCurrentTimeInMilliseconds(clock, 0);
    ASSERT_EQ(0, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    ASSERT_EQ(0, App_SharedClock_GetCurrentTimeInSeconds(clock));
}

TEST_F(ClockTest, get_previous_time_at_zero_second)
{
    App_SharedClock_SetPreviousTimeInMilliseconds(clock, 0);
    ASSERT_EQ(0, App_SharedClock_GetPreviousTimeInMilliseconds(clock));
    ASSERT_EQ(0, App_SharedClock_GetPreviousTimeInSeconds(clock));
}

TEST_F(ClockTest, get_current_time_at_nonzero_second)
{
    App_SharedClock_SetCurrentTimeInMilliseconds(clock, 1234);
    ASSERT_EQ(1234, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    ASSERT_EQ(1, App_SharedClock_GetCurrentTimeInSeconds(clock));
}

TEST_F(ClockTest, get_previous_time_at_nonzero_second)
{
    App_SharedClock_SetPreviousTimeInMilliseconds(clock, 1234);
    ASSERT_EQ(1234, App_SharedClock_GetPreviousTimeInMilliseconds(clock));
    ASSERT_EQ(1, App_SharedClock_GetPreviousTimeInSeconds(clock));
}

TEST_F(ClockTest, check_current_time_truncation)
{
    // Less than one second
    App_SharedClock_SetCurrentTimeInMilliseconds(clock, 999);
    ASSERT_EQ(999, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    ASSERT_EQ(0, App_SharedClock_GetCurrentTimeInSeconds(clock));

    // Exactly one second
    App_SharedClock_SetCurrentTimeInMilliseconds(clock, 1000);
    ASSERT_EQ(1000, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    ASSERT_EQ(1, App_SharedClock_GetCurrentTimeInSeconds(clock));

    // More than one second
    App_SharedClock_SetCurrentTimeInMilliseconds(clock, 1001);
    ASSERT_EQ(1001, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    ASSERT_EQ(1, App_SharedClock_GetCurrentTimeInSeconds(clock));
}

TEST_F(ClockTest, check_previous_time_truncation)
{
    // Less than one second
    App_SharedClock_SetPreviousTimeInMilliseconds(clock, 999);
    ASSERT_EQ(999, App_SharedClock_GetPreviousTimeInMilliseconds(clock));
    ASSERT_EQ(0, App_SharedClock_GetPreviousTimeInSeconds(clock));

    // Exactly one second
    App_SharedClock_SetPreviousTimeInMilliseconds(clock, 1000);
    ASSERT_EQ(1000, App_SharedClock_GetPreviousTimeInMilliseconds(clock));
    ASSERT_EQ(1, App_SharedClock_GetPreviousTimeInSeconds(clock));

    // More than one second
    App_SharedClock_SetPreviousTimeInMilliseconds(clock, 1001);
    ASSERT_EQ(1001, App_SharedClock_GetPreviousTimeInMilliseconds(clock));
    ASSERT_EQ(1, App_SharedClock_GetPreviousTimeInSeconds(clock));
}

TEST_F(ClockTest, check_current_time_overflow)
{
    // When the clock overflows, the last second is slightly shorter. Check that
    // the behaviour is well-defined as the clock approaches overflow.
    App_SharedClock_SetCurrentTimeInMilliseconds(clock, 4294966999);
    ASSERT_EQ(4294966999, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    ASSERT_EQ(4294966, App_SharedClock_GetCurrentTimeInSeconds(clock));

    // The for-loop intentionally uses a 64-bit value so the for-loop will
    // terminate correctly.
    for (uint64_t ms = 4294967000; ms <= UINT32_MAX; ms++)
    {
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, ms);
        ASSERT_EQ(ms, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
        ASSERT_EQ(4294967, App_SharedClock_GetCurrentTimeInSeconds(clock));
    }
}

TEST_F(ClockTest, check_previous_time_overflow)
{
    // When the clock overflows, the last second is slightly shorter. Check that
    // the behaviour is well-defined as the clock approaches overflow.
    App_SharedClock_SetPreviousTimeInMilliseconds(clock, 4294966999);
    ASSERT_EQ(4294966999, App_SharedClock_GetPreviousTimeInMilliseconds(clock));
    ASSERT_EQ(4294966, App_SharedClock_GetPreviousTimeInSeconds(clock));

    // The for-loop intentionally uses a 64-bit value so the for-loop will
    // terminate correctly.
    for (uint64_t ms = 4294967000; ms <= UINT32_MAX; ms++)
    {
        App_SharedClock_SetPreviousTimeInMilliseconds(clock, ms);
        ASSERT_EQ(ms, App_SharedClock_GetPreviousTimeInMilliseconds(clock));
        ASSERT_EQ(4294967, App_SharedClock_GetPreviousTimeInSeconds(clock));
    }
}
