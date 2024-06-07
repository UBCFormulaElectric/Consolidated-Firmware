#include <gtest/gtest.h>

#include "fake_io_time.hpp"

extern "C"
{
#include "app_signal.h"
#include "app_timer.h"
}

class SignalTest : public testing::Test
{
  protected:
    // The given entry and exit durations are more-or-less arbitrarily chosen.
    // Should be enough duration to give us confidence in the test without making the test too slow.
    const uint32_t ENTRY_DURATION_HIGH_MS = 1000;
    const uint32_t EXIT_DURATION_HIGH_MS  = 500;
    // The number of multiple cycles is more-or-less arbitrarily chosen. It should be enough
    // cycles and time to give us confidence in the test without making the test too slow.
    const uint32_t MULTIPLE_CYCLES = 100;
    // The duration of the non-zero ms delay is arbitrarily chosen. It should be
    // enough time to give us confidence in the test without making the test too slow.
    const uint32_t NON_ZERO_MS_DELAY = 100;

    // functions
    void SetUp() override
    {
        current_ms = 0;
        fake_io_time_getCurrentMs_returns(0);
        app_signal_init(&signal, ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS);
    }

    void LetTimePass()
    {
        current_ms++;
        fake_io_time_getCurrentMs_returns(current_ms);
    }

    Signal   signal;
    uint32_t current_ms = 0;
};

TEST_F(SignalTest, stays_low)
{
    for (int i = 0; i <= 2000; i++)
    {
        ASSERT_EQ(app_signal_getState(&signal, false, true), SIGNAL_STATE_CLEAR);
        LetTimePass();
    }
}

TEST_F(SignalTest, stays_high)
{
    for (uint32_t i = 0; i < 1000; i++)
    {
        SignalState correct_state = (i < ENTRY_DURATION_HIGH_MS) ? SIGNAL_STATE_CLEAR : SIGNAL_STATE_ACTIVE;
        ASSERT_EQ(app_signal_getState(&signal, true, false), correct_state);
        LetTimePass();
    }
}

TEST_F(SignalTest, cycle_test)
{
    for (uint32_t i = 0; i < ENTRY_DURATION_HIGH_MS; i++)
    {
        ASSERT_EQ(app_signal_getState(&signal, true, false), SIGNAL_STATE_CLEAR);
        LetTimePass();
    }

    ASSERT_EQ(app_signal_getState(&signal, true, false), SIGNAL_STATE_ACTIVE);

    for (uint32_t i = 0; i < EXIT_DURATION_HIGH_MS; i++)
    {
        ASSERT_EQ(app_signal_getState(&signal, false, true), SIGNAL_STATE_ACTIVE);
        LetTimePass();
    }

    ASSERT_EQ(app_signal_getState(&signal, false, true), SIGNAL_STATE_CLEAR);
}

TEST_F(SignalTest, interrupt_stays_low)
{
    for (int i = 0; i <= 200; i++)
    {
        bool flag = current_ms < ENTRY_DURATION_HIGH_MS;
        ASSERT_EQ(app_signal_getState(&signal, flag, !flag), SIGNAL_STATE_CLEAR);
        LetTimePass();
    }
}

TEST_F(SignalTest, interrupt_then_high)
{
    for (uint32_t i = 0; i <= ENTRY_DURATION_HIGH_MS; i++)
    {
        bool flag = current_ms < ENTRY_DURATION_HIGH_MS;
        ASSERT_EQ(app_signal_getState(&signal, flag, !flag), SIGNAL_STATE_CLEAR);
    }

    for (uint32_t i = 0; i <= ENTRY_DURATION_HIGH_MS; i++)
    {
        SignalState correct_state = i < ENTRY_DURATION_HIGH_MS ? SIGNAL_STATE_CLEAR : SIGNAL_STATE_ACTIVE;
        ASSERT_EQ(app_signal_getState(&signal, true, false), correct_state);
        LetTimePass();
    }
}