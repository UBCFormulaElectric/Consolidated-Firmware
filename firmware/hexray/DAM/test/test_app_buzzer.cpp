#include "app_buzzer.hpp"

#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "io_buzzerFake.hpp"
#include "io_timeFake.hpp"

#include <gtest/gtest.h>

namespace
{
constexpr uint32_t kTickPeriodMs = 10;   // 100 Hz
constexpr uint32_t kPulseMs      = 1000; // matches PULSE_DURATION_MS in app_buzzer.cpp

uint32_t s_test_time_ms = 0;

// Advance fake time and call tick() once per 100 Hz period covered by the elapsed window.
void advance(uint32_t ms)
{
    const uint32_t end = s_test_time_ms + ms;
    while (s_test_time_ms < end)
    {
        s_test_time_ms += kTickPeriodMs;
        fakes::time::setTime(s_test_time_ms);
        app::buzzer::tick();
    }
}

void setDriveState(bool in_drive)
{
    app::can_rx::VC_State_update(
        in_drive ? app::can_utils::VCState::VC_DRIVE_STATE
                 : app::can_utils::VCState::VC_INIT_STATE);
}

class AppBuzzer : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Start at t=1ms so timers behave sanely (project convention).
        s_test_time_ms = 1;
        fakes::time::setTime(s_test_time_ms);
        fakes::buzzer::reset();
        setDriveState(false);
        app::buzzer::init();
    }
};
} // namespace

// --- boot / steady state -------------------------------------------------------

TEST_F(AppBuzzer, StartsDisabled)
{
    // init() should leave the buzzer off.
    EXPECT_FALSE(fakes::buzzer::isEnabled());
}

TEST_F(AppBuzzer, StaysSilentWithoutDriveState)
{
    setDriveState(false);
    advance(5000);
    EXPECT_FALSE(fakes::buzzer::isEnabled());
    EXPECT_EQ(fakes::buzzer::enableCount(), 0u);
}

TEST_F(AppBuzzer, FiresOnBootIfVcAlreadyCommandingDriveState)
{
    // Policy: the buzzer should be on whenever VC commands drive state, including
    // the case where the DAM boots into an already-active drive state. init()
    // leaves the timer in IDLE so the first tick with condition=true restarts it.
    setDriveState(true);
    advance(kTickPeriodMs);
    EXPECT_TRUE(fakes::buzzer::isEnabled());
}

// --- rising-edge pulse ---------------------------------------------------------

TEST_F(AppBuzzer, RisingEdgeFiresFor1Second)
{
    // Hold idle for a bit, then enter drive.
    advance(500);
    ASSERT_FALSE(fakes::buzzer::isEnabled());

    setDriveState(true);

    // First tick after entering drive should enable the buzzer.
    advance(kTickPeriodMs);
    EXPECT_TRUE(fakes::buzzer::isEnabled());

    // Still on just before the 1s expiry.
    advance(kPulseMs - 2 * kTickPeriodMs);
    EXPECT_TRUE(fakes::buzzer::isEnabled());

    // After expiry, buzzer turns off even though we're still in drive state.
    advance(2 * kTickPeriodMs);
    EXPECT_FALSE(fakes::buzzer::isEnabled());
}

TEST_F(AppBuzzer, BuzzerStaysOffWhileHeldInDriveStatePastPulse)
{
    setDriveState(true);
    advance(kPulseMs + 500);

    const unsigned int enables_after_first_pulse = fakes::buzzer::enableCount();

    // Keep holding drive state for several more seconds.
    advance(5000);

    // No additional enables — we should remain silent.
    EXPECT_EQ(fakes::buzzer::enableCount(), enables_after_first_pulse);
    EXPECT_FALSE(fakes::buzzer::isEnabled());
}

// --- re-arm --------------------------------------------------------------------

TEST_F(AppBuzzer, LeavingAndReenteringDriveStateReArmsPulse)
{
    // First pulse.
    setDriveState(true);
    advance(kTickPeriodMs);
    ASSERT_TRUE(fakes::buzzer::isEnabled());
    advance(kPulseMs + kTickPeriodMs);
    ASSERT_FALSE(fakes::buzzer::isEnabled());
    const unsigned int enables_after_first = fakes::buzzer::enableCount();

    // Drop out of drive state.
    setDriveState(false);
    advance(500);
    EXPECT_FALSE(fakes::buzzer::isEnabled());

    // Re-enter drive state. Buzzer should chirp again.
    setDriveState(true);
    advance(kTickPeriodMs);
    EXPECT_TRUE(fakes::buzzer::isEnabled());
    EXPECT_GT(fakes::buzzer::enableCount(), enables_after_first);

    advance(kPulseMs + kTickPeriodMs);
    EXPECT_FALSE(fakes::buzzer::isEnabled());
}

// --- non-drive VC states are no-ops --------------------------------------------

TEST_F(AppBuzzer, OtherVcStatesDoNotFireBuzzer)
{
    for (auto state : {
             app::can_utils::VCState::VC_INIT_STATE,
             app::can_utils::VCState::VC_BMS_ON_STATE,
             app::can_utils::VCState::VC_INVERTER_ON_STATE,
         })
    {
        app::can_rx::VC_State_update(state);
        advance(2 * kPulseMs);
    }
    EXPECT_FALSE(fakes::buzzer::isEnabled());
    EXPECT_EQ(fakes::buzzer::enableCount(), 0u);
}
