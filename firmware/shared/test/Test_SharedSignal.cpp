#include "Test_Shared.h"

extern "C"
{
#include "App_SharedSignal.h"
#include "App_Timer.h"
}

class SharedSignalTest : public testing::Test
{
  protected:
    // The given entry and exit durations are more-or-less arbitrarily chosen.
    // Should be enough duration to give us confidence in the test without making the test too slow.
    const uint32_t ENTRY_DURATION_HIGH_MS = 10;
    const uint32_t EXIT_DURATION_HIGH_MS  = 10;
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
        App_Timer_SetCurrentTimeMS(current_ms);
        signal = App_SharedSignal_Create(ENTRY_DURATION_HIGH_MS, EXIT_DURATION_HIGH_MS);
    }
    void TearDown() override { TearDownObject(signal, App_SharedSignal_Destroy); }

    void updateClock()
    {
        current_ms++;
        App_Timer_SetCurrentTimeMS(current_ms);
    }

    struct Signal *   signal{};
    SignalState       currentState{};
    struct TestWorld *world{};
    uint32_t          current_ms = 0;
};

TEST_F(SharedSignalTest, stays_low)
{
    for (int i = 0; i <= 2000; i++)
    {
        currentState = App_SharedSignal_Update(signal, false, true);
        ASSERT_EQ(currentState, SIGNAL_STATE_CLEAR);
        updateClock();
    }
}

TEST_F(SharedSignalTest, stays_high)
{
    for (uint32_t i = 0; i < 1000; i++)
    {
        currentState             = App_SharedSignal_Update(signal, true, false);
        SignalState correctState = (i < ENTRY_DURATION_HIGH_MS) ? SIGNAL_STATE_CLEAR : SIGNAL_STATE_ACTIVE;
        ASSERT_EQ(currentState, correctState);
        updateClock();
    }
}

TEST_F(SharedSignalTest, cycle_test)
{
    for (uint32_t i = 0; i < ENTRY_DURATION_HIGH_MS; i++)
    {
        currentState = App_SharedSignal_Update(signal, true, false);
        ASSERT_EQ(currentState, SIGNAL_STATE_CLEAR);
        updateClock();
    }
    currentState = App_SharedSignal_Update(signal, true, false);
    ASSERT_EQ(currentState, SIGNAL_STATE_ACTIVE);
    for (uint32_t i = 0; i < EXIT_DURATION_HIGH_MS; i++)
    {
        currentState = App_SharedSignal_Update(signal, false, true);
        ASSERT_EQ(currentState, SIGNAL_STATE_ACTIVE);
        updateClock();
    }
    currentState = App_SharedSignal_Update(signal, false, true);
    ASSERT_EQ(currentState, SIGNAL_STATE_CLEAR);
}

TEST_F(SharedSignalTest, interrupt_stays_low)
{
    for (int i = 0; i <= 200; i++)
    {
        bool flag    = current_ms < ENTRY_DURATION_HIGH_MS;
        currentState = App_SharedSignal_Update(signal, flag, !flag);
        ASSERT_EQ(currentState, SIGNAL_STATE_CLEAR);
        updateClock();
    }
}

TEST_F(SharedSignalTest, interrupt_then_high)
{
    for (uint32_t i = 0; i <= ENTRY_DURATION_HIGH_MS; i++)
    {
        bool flag    = current_ms < ENTRY_DURATION_HIGH_MS;
        currentState = App_SharedSignal_Update(signal, flag, !flag);
        ASSERT_EQ(currentState, SIGNAL_STATE_CLEAR);
        updateClock();
    }
    for (uint32_t i = 0; i <= ENTRY_DURATION_HIGH_MS; i++)
    {
        currentState             = App_SharedSignal_Update(signal, true, false);
        SignalState correctState = i < ENTRY_DURATION_HIGH_MS ? SIGNAL_STATE_CLEAR : SIGNAL_STATE_ACTIVE;
        ASSERT_EQ(currentState, correctState);
        updateClock();
    }
}