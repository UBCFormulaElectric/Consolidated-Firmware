#include "Test_Shared.h"

extern "C"
{
#include "App_SharedSignal.h"
}

FAKE_VALUE_FUNC(bool, is_entry_high, struct World *);
FAKE_VALUE_FUNC(bool, is_exit_high, struct World *);
FAKE_VOID_FUNC(callback_function, struct World *);

class SharedSignalTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        callback.function                         = callback_function;
        callback.entry_condition_high_duration_ms = 0;
        callback.exit_condition_high_duration_ms  = 0;
        signal = App_SharedSignal_Create(0, is_entry_high, is_exit_high, world, callback);

        RESET_FAKE(is_entry_high);
        RESET_FAKE(is_exit_high);
        RESET_FAKE(callback_function);
    }

    void TearDown() override { TearDownObject(signal, App_SharedSignal_Destroy); }

    void SetInitialTimeAndDuration(
        uint32_t initial_time_ms,
        uint32_t entry_high_duration_ms,
        uint32_t exit_high_duration_ms)
    {
        TearDownObject(signal, App_SharedSignal_Destroy);

        callback.function                         = callback_function;
        callback.entry_condition_high_duration_ms = entry_high_duration_ms;
        callback.exit_condition_high_duration_ms  = exit_high_duration_ms;
        signal = App_SharedSignal_Create(initial_time_ms, is_entry_high, is_exit_high, world, callback);
    }

    void testSignalStaysHighForSomeCycles(
        uint32_t start_ms,
        uint32_t entry_high_duration_ms,
        uint32_t exit_high_duration_ms,
        uint32_t num_cycles)
    {
        SetInitialTimeAndDuration(start_ms, entry_high_duration_ms, exit_high_duration_ms);

        is_entry_high_fake.return_val = true;

        uint32_t current_ms                   = start_ms;
        uint32_t expected_callback_call_count = 0;

        // In the first cycle, only the very last Update() should trigger the
        // callback function because the signal wouldn't have remained high for
        // long enough until the last Update() of the first cycle.
        for (uint32_t i = 0; i < entry_high_duration_ms; i++)
        {
            App_SharedSignal_Update(signal, ++current_ms);
            // Is this the last Update() of the first cycle?
            if (i == entry_high_duration_ms - 1)
            {
                expected_callback_call_count++;
            }
            ASSERT_EQ(expected_callback_call_count, callback_function_fake.call_count);
            ASSERT_EQ(current_ms, App_SharedSignal_GetEntryLastTimeHighMs(signal));
            ASSERT_EQ(start_ms, App_SharedSignal_GetEntryLastTimeLowMs(signal));
        }

        // After the first cycle, every subsequent Update() should trigger the
        // callback function because the signal remains high.
        for (uint32_t cycle = 1; cycle < num_cycles; cycle++)
        {
            for (uint32_t ms = 0; ms < entry_high_duration_ms; ms++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(++expected_callback_call_count, callback_function_fake.call_count);
                ASSERT_EQ(current_ms, App_SharedSignal_GetEntryLastTimeHighMs(signal));
                ASSERT_EQ(start_ms, App_SharedSignal_GetEntryLastTimeLowMs(signal));
            }
        }
    }

    void testSignalStaysLowForSomeCycles(uint32_t start_ms, uint32_t high_duration_ms, uint32_t num_cycles)
    {
        SetInitialTimeAndDuration(start_ms, high_duration_ms, 1);

        is_entry_high_fake.return_val = false;

        uint32_t current_ms = start_ms;

        for (uint32_t cycle = 0; cycle < num_cycles; cycle++)
        {
            for (uint32_t ms = 0; ms < high_duration_ms; ms++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(0, callback_function_fake.call_count);
                ASSERT_EQ(start_ms, App_SharedSignal_GetEntryLastTimeHighMs(signal));
                ASSERT_EQ(current_ms, App_SharedSignal_GetEntryLastTimeLowMs(signal));
            }
        }
    }

    void testSignalGetsInterruptedAtTheLastMs(uint32_t start_ms, uint32_t high_duration_ms)
    {
        SetInitialTimeAndDuration(start_ms, high_duration_ms, 1);

        uint32_t current_ms = start_ms;

        for (uint32_t i = 0; i < high_duration_ms; i++)
        {
            // Is this the last millisecond?
            if (i == high_duration_ms - 1)
            {
                is_entry_high_fake.return_val = false;
            }
            else
            {
                is_entry_high_fake.return_val = true;
            }

            App_SharedSignal_Update(signal, ++current_ms);
            ASSERT_EQ(0, callback_function_fake.call_count);

            // Is this the last millisecond?
            if (i == high_duration_ms - 1)
            {
                ASSERT_EQ(current_ms - 1, App_SharedSignal_GetEntryLastTimeHighMs(signal));
                ASSERT_EQ(current_ms, App_SharedSignal_GetEntryLastTimeLowMs(signal));
            }
            else
            {
                ASSERT_EQ(current_ms, App_SharedSignal_GetEntryLastTimeHighMs(signal));
                ASSERT_EQ(start_ms, App_SharedSignal_GetEntryLastTimeLowMs(signal));
            }
        }
    }

    void testSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(
        uint32_t initial_ms,
        uint32_t high_duration_ms,
        uint32_t num_cycles)
    {
        testSignalGetsInterruptedAtTheLastMs(initial_ms, high_duration_ms);

        uint32_t start_ms                     = initial_ms + high_duration_ms;
        uint32_t current_ms                   = start_ms;
        uint32_t expected_callback_call_count = 0;

        is_entry_high_fake.return_val = true;

        // In the first cycle, only the very last Update() should trigger the
        // callback function because the signal wouldn't have remained high for
        // long enough until the last Update() in the first cycle.
        for (uint32_t i = 0; i < high_duration_ms; i++)
        {
            App_SharedSignal_Update(signal, ++current_ms);
            // Is this the last Update() of the first cycle?
            if (i == high_duration_ms - 1)
            {
                expected_callback_call_count++;
            }
            ASSERT_EQ(expected_callback_call_count, callback_function_fake.call_count);
            ASSERT_EQ(current_ms, App_SharedSignal_GetEntryLastTimeHighMs(signal));
            ASSERT_EQ(start_ms, App_SharedSignal_GetEntryLastTimeLowMs(signal));
        }

        // After the first cycle, every subsequent Update() should trigger the
        // callback function because the signal remains high.
        for (uint32_t cycle = 1; cycle < num_cycles; cycle++)
        {
            for (uint32_t i = 0; i < high_duration_ms; i++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(++expected_callback_call_count, callback_function_fake.call_count);
                ASSERT_EQ(current_ms, App_SharedSignal_GetEntryLastTimeHighMs(signal));
                ASSERT_EQ(start_ms, App_SharedSignal_GetEntryLastTimeLowMs(signal));
            }
        }
    }

    void testExitSignalStaysLowForSomeCycles(
        uint32_t entry_start_ms,
        uint32_t entry_high_duration_ms,
        uint32_t exit_high_duration_ms,
        uint32_t delay_ms_before_exit_signal,
        uint32_t num_cycles)
    {
        uint32_t current_ms = entry_high_duration_ms + entry_start_ms;

        // Set is_entry_condition_high for a single cycle to trigger the
        // callback function.
        testSignalStaysHighForSomeCycles(entry_start_ms, entry_high_duration_ms, exit_high_duration_ms, 1);

        is_entry_high_fake.return_val = false;

        // Delay for a few ms after is_entry_condition_high is low before
        // configuring the state of is_exit_condition_high.
        for (uint32_t ms = 0; ms < delay_ms_before_exit_signal; ms++)
        {
            App_SharedSignal_Update(signal, ++current_ms);
            ASSERT_TRUE(App_SharedSignal_IsCallbackTriggered(signal));
            ASSERT_EQ(entry_start_ms, App_SharedSignal_GetExitLastTimeHighMs(signal));
            ASSERT_EQ(current_ms, App_SharedSignal_GetExitLastTimeLowMs(signal));
        }

        is_exit_high_fake.return_val = false;

        // We can expect the callback function to trigger continuously for the
        // remaining cycles as is_exit_condition_high is always false.
        for (uint32_t cycle = 0; cycle < num_cycles; cycle++)
        {
            for (uint32_t ms = 0; ms < exit_high_duration_ms; ms++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_TRUE(App_SharedSignal_IsCallbackTriggered(signal));
                ASSERT_EQ(entry_start_ms, App_SharedSignal_GetExitLastTimeHighMs(signal));
                ASSERT_EQ(current_ms, App_SharedSignal_GetExitLastTimeLowMs(signal));
            }
        }
    }

    void testExitSignalStaysHighForSomeCycles(
        uint32_t entry_start_ms,
        uint32_t entry_high_duration_ms,
        uint32_t exit_high_duration_ms,
        uint32_t delay_ms_before_exit_signal,
        uint32_t num_cycles)
    {
        uint32_t current_ms = entry_start_ms + entry_high_duration_ms;

        // Set is_entry_condition_high for a single cycle to trigger the
        // callback function.
        testSignalStaysHighForSomeCycles(entry_start_ms, entry_high_duration_ms, exit_high_duration_ms, 1);
        is_entry_high_fake.return_val = false;

        // Delay for a few ms after is_entry_condition_high is low before
        // configuring the state of is_exit_condition_high.
        for (uint32_t ms = 0; ms < delay_ms_before_exit_signal; ms++)
        {
            App_SharedSignal_Update(signal, ++current_ms);
            ASSERT_TRUE(App_SharedSignal_IsCallbackTriggered(signal));
            ASSERT_EQ(entry_start_ms, App_SharedSignal_GetExitLastTimeHighMs(signal));
            ASSERT_EQ(current_ms, App_SharedSignal_GetExitLastTimeLowMs(signal));
        }

        is_exit_high_fake.return_val = true;
        uint32_t exit_start_ms       = current_ms;

        // For the first cycle after is_exit_condition_high is true, the
        // callback function is expected to be triggered at the last ms.
        for (uint32_t ms = 0; ms < exit_high_duration_ms; ms++)
        {
            if (ms <= exit_high_duration_ms - 1)
            {
                ASSERT_TRUE(App_SharedSignal_IsCallbackTriggered(signal));
            }
            else
            {
                ASSERT_FALSE(App_SharedSignal_IsCallbackTriggered(signal));
            }

            App_SharedSignal_Update(signal, ++current_ms);
            ASSERT_EQ(current_ms, App_SharedSignal_GetExitLastTimeHighMs(signal));
            ASSERT_EQ(exit_start_ms, App_SharedSignal_GetExitLastTimeLowMs(signal));
        }

        // The callback function should not be triggered for the remaining
        // cycles as is_exit_condition_high remains high for the given
        // exit_condition_high_duration_ms.
        for (uint32_t cycle = 0; cycle < num_cycles; cycle++)
        {
            for (uint32_t ms = 0; ms < exit_high_duration_ms; ms++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(current_ms, App_SharedSignal_GetExitLastTimeHighMs(signal));
                ASSERT_EQ(exit_start_ms, App_SharedSignal_GetExitLastTimeLowMs(signal));
            }

            ASSERT_EQ(false, App_SharedSignal_IsCallbackTriggered(signal));
        }
    }

    void testExitSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(
        uint32_t entry_start_ms,
        uint32_t entry_high_duration_ms,
        uint32_t exit_high_duration_ms,
        uint32_t delay_ms_before_exit_signal,
        uint32_t num_cycles)
    {
        // Set is_entry_condition_high for a single cycle to trigger the
        // callback function.
        testSignalStaysHighForSomeCycles(entry_start_ms, entry_high_duration_ms, exit_high_duration_ms, 1);

        uint32_t current_ms           = entry_start_ms + entry_high_duration_ms;
        is_entry_high_fake.return_val = false;

        // Delay for a few ms after is_entry_condition_high is low before
        // configuring the state of is_exit_condition_high.
        for (uint32_t ms = 0; ms < delay_ms_before_exit_signal; ms++)
        {
            App_SharedSignal_Update(signal, ++current_ms);
            ASSERT_TRUE(App_SharedSignal_IsCallbackTriggered(signal));
            ASSERT_EQ(entry_start_ms, App_SharedSignal_GetExitLastTimeHighMs(signal));
            ASSERT_EQ(current_ms, App_SharedSignal_GetExitLastTimeLowMs(signal));
        }

        uint32_t exit_start_ms = current_ms;

        // Interrupt exit_condition_high_duration_ms on the last ms for the
        // first cycle.
        for (uint32_t ms = 0; ms < exit_high_duration_ms; ms++)
        {
            if (ms == exit_high_duration_ms - 1)
            {
                is_exit_high_fake.return_val = false;
            }
            else
            {
                is_exit_high_fake.return_val = true;
            }

            App_SharedSignal_Update(signal, ++current_ms);

            if (ms == exit_high_duration_ms - 1)
            {
                ASSERT_EQ(current_ms - 1, App_SharedSignal_GetExitLastTimeHighMs(signal));
                ASSERT_EQ(current_ms, App_SharedSignal_GetExitLastTimeLowMs(signal));
            }
            else
            {
                ASSERT_EQ(current_ms, App_SharedSignal_GetExitLastTimeHighMs(signal));
                ASSERT_EQ(exit_start_ms, App_SharedSignal_GetExitLastTimeLowMs(signal));
            }

            // The callback function is triggered continuously as
            // is_exit_condition_high was interrupted on the last ms.
            ASSERT_TRUE(App_SharedSignal_IsCallbackTriggered(signal));
        }

        is_exit_high_fake.return_val = true;
        exit_start_ms                = exit_start_ms + exit_high_duration_ms;

        // We can expect the callback function not to be triggered for the
        // remaining cycles as as is_exit_condition_high is no longer
        // interrupted on the last ms.
        for (uint32_t cycle = 0; cycle < num_cycles; cycle++)
        {
            for (uint32_t ms = 0; ms < exit_high_duration_ms; ms++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(current_ms, App_SharedSignal_GetExitLastTimeHighMs(signal));
                ASSERT_EQ(exit_start_ms, App_SharedSignal_GetExitLastTimeLowMs(signal));
            }

            ASSERT_EQ(false, App_SharedSignal_IsCallbackTriggered(signal));
        }
    }

    void testCreateAndUpdateSignalOnTheSameMillisecond(uint32_t start_ms, uint32_t high_duration_ms)
    {
        // Make sure Update() has well-defined behaviour if it's called on the
        // same millisecond as when the signal was created
        SetInitialTimeAndDuration(start_ms, high_duration_ms, 1);
        App_SharedSignal_Update(signal, start_ms);
        ASSERT_EQ(0, callback_function_fake.call_count);
        ASSERT_EQ(start_ms, App_SharedSignal_GetEntryLastTimeHighMs(signal));
        ASSERT_EQ(start_ms, App_SharedSignal_GetEntryLastTimeLowMs(signal));
    }

    struct Signal *       signal;
    struct SignalCallback callback;
    struct TestWorld *    world;

    // The given entry and exit durations are more-or-less arbitrarily chosen.
    // It should be enough duration to give us confidence in the test without
    // making the test too slow.
    const uint32_t ENTRY_DURATION_HIGH_MS = 10;
    const uint32_t EXIT_DURATION_HIGH_MS  = 10;

    // The number of multiple cycles is more-or-less arbitrarily chosen. It
    // should be enough cycles and time to give us confidence in the test
    // without making the test too slow.
    const uint32_t MULTIPLE_CYCLES = 100;

    // The duration of the non zero ms delay is arbitrarily chosen. It should be
    // enough time to give us confidence in the test without making the test too
    // slow.
    const uint32_t NON_ZERO_MS_DELAY = 100;
};

TEST_F(SharedSignalTest, signal_stays_high_with_zero_start_time_for_one_cycle)
{
    testSignalStaysHighForSomeCycles(0, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 1);
}

TEST_F(SharedSignalTest, signal_stays_high_with_zero_start_time_for_multiple_cycles)
{
    testSignalStaysHighForSomeCycles(0, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_high_with_nonzero_start_time_for_one_cyle)
{
    testSignalStaysHighForSomeCycles(1, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 1);
}

TEST_F(SharedSignalTest, signal_stays_high_with_nonzero_start_time_for_multiple_cycles)
{
    testSignalStaysHighForSomeCycles(1, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_high_with_timer_overflow_for_one_cycle)
{
    testSignalStaysHighForSomeCycles(UINT32_MAX - 1, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 1);
}

TEST_F(SharedSignalTest, signal_stays_high_for_multiple_cycles_with_timer_overflow_for_multiple_cycles)
{
    testSignalStaysHighForSomeCycles(UINT32_MAX - 1, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_low_with_zero_start_time_for_one_cycle)
{
    testSignalStaysLowForSomeCycles(0, ENTRY_DURATION_HIGH_MS, 1);
}

TEST_F(SharedSignalTest, signal_stays_low_with_zero_start_time_for_multiple_cycles)
{
    testSignalStaysLowForSomeCycles(0, ENTRY_DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_low_with_timer_overflow_for_one_cycle)
{
    testSignalStaysLowForSomeCycles(UINT32_MAX - 1, ENTRY_DURATION_HIGH_MS, 1);
}

TEST_F(SharedSignalTest, signal_stays_low_with_timer_overflow_for_multiple_cycles)
{
    testSignalStaysLowForSomeCycles(UINT32_MAX - 1, ENTRY_DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_gets_interrupted_with_zero_start_time)
{
    testSignalGetsInterruptedAtTheLastMs(0, ENTRY_DURATION_HIGH_MS);
}

TEST_F(SharedSignalTest, signal_gets_interrupted_with_nonzero_start_time)
{
    testSignalGetsInterruptedAtTheLastMs(1, ENTRY_DURATION_HIGH_MS);
}

TEST_F(SharedSignalTest, signal_gets_interrupted_with_timer_overflow)
{
    testSignalGetsInterruptedAtTheLastMs(UINT32_MAX - 1, ENTRY_DURATION_HIGH_MS);
}

TEST_F(SharedSignalTest, signal_gets_interrupted_with_zero_start_time_and_then_stays_high_for_multiple_cycles)
{
    testSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(0, ENTRY_DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_gets_interrupted_with_nonzero_start_time_and_then_stays_high_for_multiple_cycles)
{
    testSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(1, ENTRY_DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, create_and_update_signal_on_the_same_millisecond_with_zero_start_time)
{
    testCreateAndUpdateSignalOnTheSameMillisecond(0, ENTRY_DURATION_HIGH_MS);
}

TEST_F(SharedSignalTest, create_and_update_signal_on_the_same_millisecond_with_nonzero_start_time)
{
    testCreateAndUpdateSignalOnTheSameMillisecond(1, ENTRY_DURATION_HIGH_MS);
}

TEST_F(SharedSignalTest, zero_ms_delay_before_exit_signal_stays_low_for_one_cycle)
{
    testExitSignalStaysLowForSomeCycles(0, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, 1);
}

TEST_F(SharedSignalTest, non_zero_ms_delay_before_exit_signal_stays_low_for_one_cycle)
{
    testExitSignalStaysLowForSomeCycles(0, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, NON_ZERO_MS_DELAY, 1);
}

TEST_F(SharedSignalTest, exit_signal_stays_low_with_overflow_for_one_cycle)
{
    testExitSignalStaysLowForSomeCycles(
        UINT32_MAX - ENTRY_DURATION_HIGH_MS, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, 1);
}

TEST_F(SharedSignalTest, exit_signal_stays_low_with_overflow_for_multiple_cycles)
{
    testExitSignalStaysLowForSomeCycles(
        UINT32_MAX - ENTRY_DURATION_HIGH_MS, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, zero_ms_delay_before_exit_signal_stays_high_for_one_cycle)
{
    testExitSignalStaysHighForSomeCycles(0, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, 1);
}

TEST_F(SharedSignalTest, non_zero_ms_delay_before_exit_signal_stays_high_for_multiple_cycles)
{
    testExitSignalStaysHighForSomeCycles(
        0, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, NON_ZERO_MS_DELAY, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, exit_signal_stays_high_with_overflow_for_one_cycle)
{
    testExitSignalStaysHighForSomeCycles(
        UINT32_MAX - ENTRY_DURATION_HIGH_MS, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, 1);
}

TEST_F(SharedSignalTest, exit_signal_stays_high_with_overflow_for_multiple_cycles)
{
    testExitSignalStaysHighForSomeCycles(
        UINT32_MAX - ENTRY_DURATION_HIGH_MS, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, zero_ms_delay_before_exit_signal_gets_interrupted)
{
    testExitSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(
        0, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, 0);
}

TEST_F(SharedSignalTest, non_zero_ms_delay_before_exit_signal_gets_interrupted)
{
    testExitSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(
        0, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, NON_ZERO_MS_DELAY, 0);
}

TEST_F(SharedSignalTest, exit_signal_gets_interrupted_and_then_stays_high_for_multiple_cycles)
{
    testExitSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(
        0, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, exit_signal_gets_interrupted_at_last_ms_with_timer_overflow)
{
    testExitSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(
        UINT32_MAX - ENTRY_DURATION_HIGH_MS, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, 0);
}

TEST_F(SharedSignalTest, exit_signal_gets_interrupted_at_last_ms_with_timer_overflow_for_multiple_cycles)
{
    testExitSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(
        UINT32_MAX - ENTRY_DURATION_HIGH_MS, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS, 0, MULTIPLE_CYCLES);
}
