#include "Test_Shared.h"

extern "C"
{
#include "App_SharedWaitSignal.h"
}

FAKE_VALUE_FUNC(bool, is_wait_high, struct World *);
FAKE_VOID_FUNC(wait_callback_function, struct World *);

class SharedWaitSignalTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        callback.function         = wait_callback_function;
        callback.wait_duration_ms = 0;
        wait_signal               = App_SharedWaitSignal_Create(0, is_wait_high, world, callback);

        RESET_FAKE(is_wait_high);
        RESET_FAKE(wait_callback_function);
    }

    void TearDown() override { TearDownObject(wait_signal, App_SharedWaitSignal_Destroy); }

    void SetInitialTimeAndDuration(uint32_t initial_time_ms, uint32_t wait_duration_ms)
    {
        TearDownObject(wait_signal, App_SharedWaitSignal_Destroy);

        callback.function         = wait_callback_function;
        callback.wait_duration_ms = wait_duration_ms;
        wait_signal               = App_SharedWaitSignal_Create(initial_time_ms, is_wait_high, world, callback);
    }

    void testCreateAndUpdateWaitSignalOnTheSameMillisecond(uint32_t start_ms, uint32_t wait_duration_ms)
    {
        SetInitialTimeAndDuration(start_ms, wait_duration_ms);
        App_SharedWaitSignal_Update(wait_signal, start_ms);
        ASSERT_EQ(0, wait_callback_function_fake.call_count);
        ASSERT_EQ(start_ms, App_SharedWaitSignal_GetLastTimeHighMs(wait_signal));
    }

    void testWaitSignalStatusHighTriggersWaitForSomeMs(
        uint32_t start_ms,
        uint32_t first_wait_duration_ms,
        uint32_t second_wait_duration_ms)
    {
        SetInitialTimeAndDuration(start_ms, first_wait_duration_ms);

        is_wait_high_fake.return_val = true;

        uint32_t expected_wait_callback_call_count = 0;
        uint32_t current_ms                        = start_ms;

        for (uint32_t ms = 0; ms < first_wait_duration_ms - 1; ms++)
        {
            App_SharedWaitSignal_Update(wait_signal, ++current_ms);
            ASSERT_EQ(start_ms + 1, App_SharedWaitSignal_GetLastTimeHighMs(wait_signal));
            ASSERT_TRUE(App_SharedWaitSignal_IsWaiting(wait_signal));
            ASSERT_EQ(expected_wait_callback_call_count, wait_callback_function_fake.call_count);

            if (ms == 0)
            {
                // Set the signal low after the first cycle
                is_wait_high_fake.return_val = false;
            }
        }

        App_SharedWaitSignal_Update(wait_signal, ++current_ms);
        expected_wait_callback_call_count++;
        ASSERT_EQ(start_ms + 1, App_SharedWaitSignal_GetLastTimeHighMs(wait_signal));

        // Check that the callback function is triggered once after waiting for
        // first_wait_duration_ms, and check that the wait signal is low.
        ASSERT_EQ(expected_wait_callback_call_count, wait_callback_function_fake.call_count);
        ASSERT_FALSE(App_SharedWaitSignal_IsWaiting(wait_signal));

        // Check that the callback function does not get triggered more than
        // once, and check that the wait signal remains low for the remaining
        // ms.
        for (uint32_t ms = 0; ms < first_wait_duration_ms; ms++)
        {
            App_SharedWaitSignal_Update(wait_signal, ++current_ms);
            ASSERT_FALSE(App_SharedWaitSignal_IsWaiting(wait_signal));
            ASSERT_EQ(expected_wait_callback_call_count, wait_callback_function_fake.call_count);
        }
    }

    void testWaitSignalStaysLowDoesNotTriggerWaitForSomeMs(
        uint32_t start_ms,
        uint32_t first_wait_duration_ms,
        uint32_t second_wait_duration_ms)
    {
        SetInitialTimeAndDuration(start_ms, first_wait_duration_ms);

        is_wait_high_fake.return_val = false;

        uint32_t expected_wait_callback_call_count = 0;
        uint32_t current_ms                        = start_ms;

        // We do not expect a wait to be triggered as the wait signal remains
        // low for the entirety of this test.
        for (uint32_t ms = 0; ms < first_wait_duration_ms + second_wait_duration_ms; ms++)
        {
            App_SharedWaitSignal_Update(wait_signal, ++current_ms);
            ASSERT_EQ(start_ms, App_SharedWaitSignal_GetLastTimeHighMs(wait_signal));
            ASSERT_FALSE(App_SharedWaitSignal_IsWaiting(wait_signal));
            ASSERT_EQ(expected_wait_callback_call_count, wait_callback_function_fake.call_count);
        }
    }

    struct WaitSignal *       wait_signal;
    struct WaitSignalCallback callback;
    struct TestWorld *        world;

    // The duration is more-or-less arbitrarily chosen. It should be enough
    // duration to give us confidence in the test without making the test too
    // slow.
    const uint32_t WAIT_DURATION_MS = 100;
};

TEST_F(SharedWaitSignalTest, create_and_update_signal_on_the_same_millisecond_with_zero_start_time)
{
    testCreateAndUpdateWaitSignalOnTheSameMillisecond(0, 0);
}

TEST_F(SharedWaitSignalTest, create_and_update_signal_on_the_same_millisecond_with_non_zero_start_time)
{
    testCreateAndUpdateWaitSignalOnTheSameMillisecond(1, 0);
}

TEST_F(SharedWaitSignalTest, wait_signal_high_triggers_wait_with_zero_start_time)
{
    testWaitSignalStatusHighTriggersWaitForSomeMs(0, WAIT_DURATION_MS, 0);
}

TEST_F(SharedWaitSignalTest, wait_signal_high_triggers_wait_with_non_zero_start_time)
{
    testWaitSignalStatusHighTriggersWaitForSomeMs(1, WAIT_DURATION_MS, 0);
}

TEST_F(SharedWaitSignalTest, wait_signal_high_triggers_wait_with_non_zero_start_time_with_overflow)
{
    testWaitSignalStatusHighTriggersWaitForSomeMs(UINT32_MAX - 1, WAIT_DURATION_MS, 0);
}

TEST_F(SharedWaitSignalTest, wait_signal_high_triggers_wait_with_zero_start_time_and_keep_waiting_after)
{
    testWaitSignalStatusHighTriggersWaitForSomeMs(0, WAIT_DURATION_MS, WAIT_DURATION_MS);
}

TEST_F(SharedWaitSignalTest, wait_signal_high_triggers_wait_with_non_zero_start_time_and_keep_waiting_after)
{
    testWaitSignalStatusHighTriggersWaitForSomeMs(1, WAIT_DURATION_MS, WAIT_DURATION_MS);
}

TEST_F(
    SharedWaitSignalTest,
    wait_signal_high_triggers_wait_with_non_zero_start_time_with_overflow_and_keep_waiting_after)
{
    testWaitSignalStatusHighTriggersWaitForSomeMs(UINT32_MAX - 1, WAIT_DURATION_MS, WAIT_DURATION_MS);
}

TEST_F(SharedWaitSignalTest, wait_signal_low_does_not_trigger_wait_with_zero_start_time)
{
    testWaitSignalStaysLowDoesNotTriggerWaitForSomeMs(0, WAIT_DURATION_MS, 0);
}

TEST_F(SharedWaitSignalTest, wait_signal_low_does_not_trigger_wait_with_non_zero_start_time)
{
    testWaitSignalStaysLowDoesNotTriggerWaitForSomeMs(1, WAIT_DURATION_MS, 0);
}

TEST_F(SharedWaitSignalTest, wait_signal_low_does_not_trigger_wait_with_non_zero_time_with_overflow)
{
    testWaitSignalStaysLowDoesNotTriggerWaitForSomeMs(UINT32_MAX - 1, WAIT_DURATION_MS, 0);
}

TEST_F(SharedWaitSignalTest, wait_signal_low_does_not_trigger_wait_with_zero_start_time_and_keep_waiting_after)
{
    testWaitSignalStaysLowDoesNotTriggerWaitForSomeMs(0, WAIT_DURATION_MS, WAIT_DURATION_MS);
}

TEST_F(SharedWaitSignalTest, wait_signal_low_does_not_trigger_wait_with_non_zero_start_time_and_keep_waiting_after)
{
    testWaitSignalStaysLowDoesNotTriggerWaitForSomeMs(1, WAIT_DURATION_MS, WAIT_DURATION_MS);
}

TEST_F(
    SharedWaitSignalTest,
    wait_signal_low_does_not_trigger_wait_with_non_zero_start_time_with_overflow_and_keep_waiting_after)
{
    testWaitSignalStaysLowDoesNotTriggerWaitForSomeMs(UINT32_MAX - 1, WAIT_DURATION_MS, WAIT_DURATION_MS);
}
