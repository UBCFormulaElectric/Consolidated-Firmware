#include "Test_Shared.h"

extern "C"
{
#include "App_SharedSignal.h"
}

FAKE_VALUE_FUNC(bool, is_high);
FAKE_VOID_FUNC(callback);

class SharedSignalTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        signal = App_SharedSignal_Create(is_high, 0, 0, callback);

        RESET_FAKE(is_high);
        RESET_FAKE(callback);
    }

    void TearDown() override
    {
        TearDownObject(signal, App_SharedSignal_Destroy);
    }

    void SetInitialTimeAndDuration(
        uint32_t initial_time_ms,
        uint32_t duration_high_ms)
    {
        TearDownObject(signal, App_SharedSignal_Destroy);
        signal = App_SharedSignal_Create(
            is_high, initial_time_ms, duration_high_ms, callback);
    }

    void testSignalStaysHighForSomeCycles(
        uint32_t start_ms,
        uint32_t duration_high_ms,
        uint32_t num_cycles)
    {
        SetInitialTimeAndDuration(start_ms, duration_high_ms);

        is_high_fake.return_val = true;

        uint32_t current_ms                   = start_ms;
        uint32_t expected_callback_call_count = 0;

        // In the first cycle, only the very last update should trigger the
        // callback function because the signal wouldn't have remained high for
        // long enough until the last update in the first cycle.
        for (uint32_t i = 0; i < duration_high_ms; i++)
        {
            App_SharedSignal_Update(signal, ++current_ms);
            if (i == duration_high_ms - 1)
            {
                expected_callback_call_count++;
            }
            ASSERT_EQ(expected_callback_call_count, callback_fake.call_count);
            ASSERT_EQ(current_ms, App_SharedSignal_GetLastTimeHighMs(signal));
            ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
        }

        // After the first cycle, every subsequent update should trigger the
        // callback function because the signal remains high.
        for (uint32_t cycle = 1; cycle < num_cycles; cycle++)
        {
            for (uint32_t ms = 0; ms < duration_high_ms; ms++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(
                    ++expected_callback_call_count, callback_fake.call_count);
                ASSERT_EQ(
                    current_ms, App_SharedSignal_GetLastTimeHighMs(signal));
                ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
            }
        }
    }

    void testSignalStaysLowForSomeCycles(
        uint32_t start_ms,
        uint32_t duration_high_ms,
        uint32_t num_cycles)
    {
        SetInitialTimeAndDuration(start_ms, duration_high_ms);

        is_high_fake.return_val = false;

        uint32_t current_ms = start_ms;

        for (uint32_t cycle = 0; cycle < num_cycles; cycle++)
        {
            for (uint32_t ms = 0; ms < duration_high_ms; ms++)
            {
                App_SharedSignal_Update(signal, ++current_ms);
                ASSERT_EQ(0, callback_fake.call_count);
                ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeHighMs(signal));
                ASSERT_EQ(
                    current_ms, App_SharedSignal_GetLastTimeLowMs(signal));
            }
        }
    }

    void testSignalStaysHighForLessThanOneCycle(
        uint32_t start_ms,
        uint32_t duration_high_ms)
    {
        SetInitialTimeAndDuration(start_ms, duration_high_ms);

        uint32_t current_ms = start_ms;

        // The callback function should not be called before a full cycle is
        // complete, even if the signal stays high.
        for (uint32_t i = 0; i < duration_high_ms; i++)
        {
            if (i == duration_high_ms - 1)
            {
                is_high_fake.return_val = false;
            }
            else
            {
                is_high_fake.return_val = true;
            }

            App_SharedSignal_Update(signal, ++current_ms);
            ASSERT_EQ(0, callback_fake.call_count);

            if (i == duration_high_ms - 1)
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

    struct Signal *signal;

    // The duration is more-or-less arbitrarily chosen. It should be enough
    // duration to give us confidence in the test without making the test too
    // slow.
    const uint32_t DURATION_MS = 10;

    // The number of multiple cycles is more-or-less arbitrarily chosen. It
    // should be enough cycles to give us confidence in the test without making
    // the test too slow.
    const uint32_t MULTIPLE_CYCLES = 100;
};

TEST_F(SharedSignalTest, signal_stays_high_with_zero_start_time_for_one_cycle)
{
    testSignalStaysHighForSomeCycles(0, DURATION_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_with_zero_start_time_for_multiple_cycles)
{
    testSignalStaysHighForSomeCycles(0, DURATION_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_high_with_nonzero_start_time_for_one_cyle)
{
    testSignalStaysHighForSomeCycles(1, DURATION_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_with_nonzero_start_time_for_multiple_cycles)
{
    testSignalStaysHighForSomeCycles(1, DURATION_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_high_with_timer_overflow_for_one_cycle)
{
    testSignalStaysHighForSomeCycles(UINT32_MAX - 1, DURATION_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_for_multiple_cycles_with_timer_overflow_for_multiple_cycles)
{
    testSignalStaysHighForSomeCycles(
        UINT32_MAX - 1, DURATION_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_low_with_zero_start_time_for_one_cycle)
{
    testSignalStaysLowForSomeCycles(0, DURATION_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_low_with_zero_start_time_for_multiple_cycles)
{
    testSignalStaysLowForSomeCycles(0, DURATION_MS, MULTIPLE_CYCLES);
}

TEST_F(SharedSignalTest, signal_stays_low_with_timer_overflow_for_one_cycle)
{
    testSignalStaysLowForSomeCycles(UINT32_MAX - 1, DURATION_MS, 1);
}

TEST_F(
    SharedSignalTest,
    signal_stays_low_with_timer_overflow_for_multiple_cycles)
{
    testSignalStaysLowForSomeCycles(
        UINT32_MAX - 1, DURATION_MS, MULTIPLE_CYCLES);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_for_less_than_one_cycle_with_zero_start_time)
{
    testSignalStaysHighForLessThanOneCycle(0, DURATION_MS);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_for_less_than_one_cycle_with_nonzero_start_time)
{
    testSignalStaysHighForLessThanOneCycle(1, DURATION_MS);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_for_less_than_one_cycle_with_timer_overflow)
{
    testSignalStaysHighForLessThanOneCycle(UINT32_MAX - 1, DURATION_MS);
}

TEST_F(
    SharedSignalTest,
    signal_stays_high_for_less_than_one_cycle_with_zero_start_time_and_then_stays_high_for_multiple_cycles)
{
    testSignalStaysHighForLessThanOneCycle(0, DURATION_MS);

    uint32_t start_ms                     = DURATION_MS;
    uint32_t current_ms                   = start_ms;
    uint32_t expected_callback_call_count = 0;

    is_high_fake.return_val = true;

    // In the first cycle, only the very last update should trigger the
    // callback function because the signal wouldn't have remained high for
    // long enough until the last update in the first cycle.
    for (uint32_t i = 0; i < DURATION_MS; i++)
    {
        App_SharedSignal_Update(signal, ++current_ms);
        if (i == DURATION_MS - 1)
        {
            expected_callback_call_count++;
        }
        ASSERT_EQ(expected_callback_call_count, callback_fake.call_count);
        ASSERT_EQ(current_ms, App_SharedSignal_GetLastTimeHighMs(signal));
        ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
    }

    // After the first cycle, every subsequent update should trigger the
    // callback function because the signal remains high.
    for (uint32_t cycle = 1; cycle < MULTIPLE_CYCLES; cycle++)
    {
        for (uint32_t i = 0; i < DURATION_MS; i++)
        {
            App_SharedSignal_Update(signal, ++current_ms);
            ASSERT_EQ(++expected_callback_call_count, callback_fake.call_count);
            ASSERT_EQ(current_ms, App_SharedSignal_GetLastTimeHighMs(signal));
            ASSERT_EQ(start_ms, App_SharedSignal_GetLastTimeLowMs(signal));
        }
    }
}

TEST_F(
    SharedSignalTest,
    update_signal_on_the_same_millisecond_as_when_the_signal_was_created_with_zero_start_time)
{
    // Make sure Update() has well-defined behaviour if it's called on the same
    // millisecond as when the signal was created
    const uint32_t START_TIME = 0;
    SetInitialTimeAndDuration(START_TIME, DURATION_MS);
    App_SharedSignal_Update(signal, START_TIME);
    ASSERT_EQ(0, callback_fake.call_count);
    ASSERT_EQ(START_TIME, App_SharedSignal_GetLastTimeHighMs(signal));
    ASSERT_EQ(START_TIME, App_SharedSignal_GetLastTimeLowMs(signal));
}

TEST_F(
    SharedSignalTest,
    update_signal_on_the_same_millisecond_as_when_the_signal_was_created_with_nonzero_start_time)
{
    // Make sure Update() has well-defined behaviour if it's called on the same
    // millisecond as when the signal was created
    const uint32_t START_TIME = 1;
    SetInitialTimeAndDuration(START_TIME, DURATION_MS);
    App_SharedSignal_Update(signal, START_TIME);
    ASSERT_EQ(0, callback_fake.call_count);
    ASSERT_EQ(START_TIME, App_SharedSignal_GetLastTimeHighMs(signal));
    ASSERT_EQ(START_TIME, App_SharedSignal_GetLastTimeLowMs(signal));
}
