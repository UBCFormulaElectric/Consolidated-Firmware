#include "Test_Shared.h"

extern "C"
{
#include "App_SharedSignal.h"
}

FAKE_VALUE_FUNC(bool, is_high, struct World *);
FAKE_VOID_FUNC(callback_function, struct World *);

class SharedSignalTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        callback.function                         = callback_function;
        callback.entry_condition_high_duration_ms = 0;
        signal = App_SharedSignal_Create(0, is_high, world, callback);

        RESET_FAKE(is_high);
        RESET_FAKE(callback_function);
    }

    void TearDown() override
    {
        TearDownObject(signal, App_SharedSignal_Destroy);
    }

    void SetInitialTimeAndDuration(
        uint32_t initial_time_ms,
        uint32_t high_duration_ms)
    {
        TearDownObject(signal, App_SharedSignal_Destroy);

        callback.function                         = callback_function;
        callback.entry_condition_high_duration_ms = high_duration_ms;
        signal =
            App_SharedSignal_Create(initial_time_ms, is_high, world, callback);
    }

    void testSignalStaysHighForSomeCycles(
        uint32_t start_ms,
        uint32_t high_duration_ms,
        uint32_t num_cycles)
    {
        SetInitialTimeAndDuration(start_ms, high_duration_ms);

        is_high_fake.return_val = true;

        uint32_t current_ms                   = start_ms;
        uint32_t expected_callback_call_count = 0;

        // In the first cycle, only the very last Update() should trigger the
        // callback function because the signal wouldn't have remained high for
        // long enough until the last Update() of the first cycle.
        for (uint32_t i = 0; i < high_duration_ms; i++)
        {
            App_SharedSignal_Update(signal, ++current_ms);
            // Is this the last Update() of the first cycle?
            if (i == high_duration_ms - 1)
            {
                expected_callback_call_count++;
            }
            ASSERT_EQ(
                expected_callback_call_count,
                callback_function_fake.call_count);
            ASSERT_EQ(current_ms, App_SharedSignal_GetLastTimeHighMs(signal));
            ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
        }

        // After the first cycle, every subsequent Update() should trigger the
        // callback function because the signal remains high.
        for (uint32_t cycle = 1; cycle < num_cycles; cycle++)
        {
            for (uint32_t ms = 0; ms < high_duration_ms; ms++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(
                    ++expected_callback_call_count,
                    callback_function_fake.call_count);
                ASSERT_EQ(
                    current_ms, App_SharedSignal_GetLastTimeHighMs(signal));
                ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
            }
        }
    }

    void testSignalStaysLowForSomeCycles(
        uint32_t start_ms,
        uint32_t high_duration_ms,
        uint32_t num_cycles)
    {
        SetInitialTimeAndDuration(start_ms, high_duration_ms);

        is_high_fake.return_val = false;

        uint32_t current_ms = start_ms;

        for (uint32_t cycle = 0; cycle < num_cycles; cycle++)
        {
            for (uint32_t ms = 0; ms < high_duration_ms; ms++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(0, callback_function_fake.call_count);
                ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeHighMs(signal));
                ASSERT_EQ(
                    current_ms, App_SharedSignal_GetLastTimeLowMs(signal));
            }
        }
    }

    void testSignalGetsInterruptedAtTheLastMs(
        uint32_t start_ms,
        uint32_t high_duration_ms)
    {
        SetInitialTimeAndDuration(start_ms, high_duration_ms);

        uint32_t current_ms = start_ms;

        for (uint32_t i = 0; i < high_duration_ms; i++)
        {
            // Is this the last millisecond?
            if (i == high_duration_ms - 1)
            {
                is_high_fake.return_val = false;
            }
            else
            {
                is_high_fake.return_val = true;
            }

            App_SharedSignal_Update(signal, ++current_ms);
            ASSERT_EQ(0, callback_function_fake.call_count);

            // Is this the last millisecond?
            if (i == high_duration_ms - 1)
            {
                ASSERT_EQ(
                    current_ms - 1, App_SharedSignal_GetLastTimeHighMs(signal));
                ASSERT_EQ(
                    current_ms, App_SharedSignal_GetLastTimeLowMs(signal));
            }
            else
            {
                ASSERT_EQ(
                    current_ms, App_SharedSignal_GetLastTimeHighMs(signal));
                ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
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

        is_high_fake.return_val = true;

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
            ASSERT_EQ(
                expected_callback_call_count,
                callback_function_fake.call_count);
            ASSERT_EQ(current_ms, App_SharedSignal_GetLastTimeHighMs(signal));
            ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
        }

        // After the first cycle, every subsequent Update() should trigger the
        // callback function because the signal remains high.
        for (uint32_t cycle = 1; cycle < num_cycles; cycle++)
        {
            for (uint32_t i = 0; i < high_duration_ms; i++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(
                    ++expected_callback_call_count,
                    callback_function_fake.call_count);
                ASSERT_EQ(
                    current_ms, App_SharedSignal_GetLastTimeHighMs(signal));
                ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
            }
        }
    }

    void testCreateAndUpdateSignalOnTheSameMillisecond(
        uint32_t start_ms,
        uint32_t high_duration_ms)
    {
        // Make sure Update() has well-defined behaviour if it's called on the
        // same millisecond as when the signal was created
        SetInitialTimeAndDuration(start_ms, high_duration_ms);
        App_SharedSignal_Update(signal, start_ms);
        ASSERT_EQ(0, callback_function_fake.call_count);
        ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeHighMs(signal));
        ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
    }

    struct Signal *       signal;
    struct SignalCallback callback;
    struct TestWorld *    world;

    // The duration is more-or-less arbitrarily chosen. It should be enough
    // duration to give us confidence in the test without making the test too
    // slow.
    const uint32_t DURATION_HIGH_MS = 10;

    // The number of multiple cycles is more-or-less arbitrarily chosen. It
    // should be enough cycles to give us confidence in the test without making
    // the test too slow.
    const uint32_t MULTIPLE_CYCLES = 100;
};

TEST_F(SharedSignalTest, signal_stays_high_with_zero_start_time_for_one_cycle)
{
    testSignalStaysHighForSomeCycles(0, DURATION_HIGH_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_with_zero_start_time_for_multiple_cycles)
{
    testSignalStaysHighForSomeCycles(0, DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_high_with_nonzero_start_time_for_one_cyle)
{
    testSignalStaysHighForSomeCycles(1, DURATION_HIGH_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_with_nonzero_start_time_for_multiple_cycles)
{
    testSignalStaysHighForSomeCycles(1, DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_high_with_timer_overflow_for_one_cycle)
{
    testSignalStaysHighForSomeCycles(UINT32_MAX - 1, DURATION_HIGH_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_for_multiple_cycles_with_timer_overflow_for_multiple_cycles)
{
    testSignalStaysHighForSomeCycles(
        UINT32_MAX - 1, DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_low_with_zero_start_time_for_one_cycle)
{
    testSignalStaysLowForSomeCycles(0, DURATION_HIGH_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_low_with_zero_start_time_for_multiple_cycles)
{
    testSignalStaysLowForSomeCycles(0, DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_low_with_timer_overflow_for_one_cycle)
{
    testSignalStaysLowForSomeCycles(UINT32_MAX - 1, DURATION_HIGH_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_low_with_timer_overflow_for_multiple_cycles)
{
    testSignalStaysLowForSomeCycles(
        UINT32_MAX - 1, DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_gets_interrupted_with_zero_start_time)
{
    testSignalGetsInterruptedAtTheLastMs(0, DURATION_HIGH_MS);
}

TEST_F(SharedSignalTest, signal_gets_interrupted_with_nonzero_start_time)
{
    testSignalGetsInterruptedAtTheLastMs(1, DURATION_HIGH_MS);
}

TEST_F(SharedSignalTest, signal_gets_interrupted_with_timer_overflow)
{
    testSignalGetsInterruptedAtTheLastMs(UINT32_MAX - 1, DURATION_HIGH_MS);
}

TEST_F(
    SharedSignalTest,
    signal_gets_interrupted_with_zero_start_time_and_then_stays_high_for_multiple_cycles)
{
    testSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(
        0, DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(
    SharedSignalTest,
    signal_gets_interrupted_with_nonzero_start_time_and_then_stays_high_for_multiple_cycles)
{
    testSignalGetsInterruptedAtTheLastMsAndThenStaysHighForSomeCycles(
        1, DURATION_HIGH_MS, MULTIPLE_CYCLES);
}

TEST_F(
    SharedSignalTest,
    create_and_update_signal_on_the_same_millisecond_with_zero_start_time)
{
    testCreateAndUpdateSignalOnTheSameMillisecond(0, DURATION_HIGH_MS);
}

TEST_F(
    SharedSignalTest,
    create_and_update_signal_on_the_same_millisecond_with_nonzero_start_time)
{
    testCreateAndUpdateSignalOnTheSameMillisecond(1, DURATION_HIGH_MS);
}
