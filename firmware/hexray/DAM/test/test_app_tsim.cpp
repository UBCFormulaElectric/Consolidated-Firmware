#include "app_tsim.hpp"

#include "app_canRx.hpp"
#include "io_timeFake.hpp"
#include "io_tsimFake.hpp"

#include <gtest/gtest.h>

namespace
{
constexpr uint32_t kTickPeriodMs    = 10;     // 100 Hz
constexpr uint32_t kToggleMs        = 150;    // matches RED_TOGGLE_TIME_MS in app_tsim.cpp
constexpr uint32_t kBootupIgnoreMs  = 3000;   // matches BOOTUP_IGNORE_TIME_MS

uint32_t s_test_time_ms = 0;

// Advance fake time and call tick() once per 100 Hz period covered by the elapsed window.
// Steps in 10 ms increments so the state machine sees a tick at every period boundary,
// matching how the real 100 Hz task would run.
void advance(uint32_t ms)
{
    const uint32_t end = s_test_time_ms + ms;
    while (s_test_time_ms < end)
    {
        s_test_time_ms += kTickPeriodMs;
        fakes::time::setTime(s_test_time_ms);
        app::tsim::tick();
    }
}

void setAllLatches(bool ok)
{
    app::can_rx::BMS_BmsLatchOk_update(ok);
    app::can_rx::BMS_ImdLatchOk_update(ok);
    app::can_rx::BMS_BspdLatchOk_update(ok);
}

class AppTsim : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Start at t=1ms so timers behave sanely (the project convention from EcuTestBase).
        s_test_time_ms = 1;
        fakes::time::setTime(s_test_time_ms);
        fakes::tsim::reset();
        setAllLatches(false);
        app::tsim::init();
    }
};
} // namespace

// --- bootup window ---------------------------------------------------------

TEST_F(AppTsim, BootupWindowForcesGreenEvenWithBadLatches)
{
    // Latches are all false from SetUp(). Without the bootup-ignore window we'd be
    // blinking red; we should see green for the full 3 s window instead.
    setAllLatches(false);

    // First tick after init() should already have set green.
    app::tsim::tick();
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::GREEN);

    // Hold green throughout the bootup window.
    advance(kBootupIgnoreMs - 100);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::GREEN);
}

TEST_F(AppTsim, AfterBootupWindowWithBadLatchesGoesToRedThenToggles)
{
    setAllLatches(false);

    // Skip past the 3 s bootup window.
    advance(kBootupIgnoreMs + kTickPeriodMs);

    // First fault entry should drive red.
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::RED);

    // Within one toggle period it should flip to OFF.
    advance(kToggleMs + kTickPeriodMs);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::OFF);

    // And then back to RED.
    advance(kToggleMs + kTickPeriodMs);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::RED);
}

// --- steady state ----------------------------------------------------------

TEST_F(AppTsim, AllLatchesOkStaysGreenForever)
{
    setAllLatches(true);

    // Past the bootup window, real latch state takes over. Green should hold.
    advance(kBootupIgnoreMs + 5000);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::GREEN);
}

TEST_F(AppTsim, AnySingleLatchFailureTriggersBlink)
{
    // BSPD bad, others ok → still a fault.
    app::can_rx::BMS_BmsLatchOk_update(true);
    app::can_rx::BMS_ImdLatchOk_update(true);
    app::can_rx::BMS_BspdLatchOk_update(false);

    advance(kBootupIgnoreMs + kTickPeriodMs);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::RED);

    advance(kToggleMs + kTickPeriodMs);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::OFF);
}

TEST_F(AppTsim, ImdFailureAloneTriggersBlink)
{
    app::can_rx::BMS_BmsLatchOk_update(true);
    app::can_rx::BMS_ImdLatchOk_update(false);
    app::can_rx::BMS_BspdLatchOk_update(true);

    advance(kBootupIgnoreMs + kTickPeriodMs);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::RED);
}

// --- transitions -----------------------------------------------------------

TEST_F(AppTsim, RecoveryFromFaultGoesGreenWithoutWaitingForToggleTimer)
{
    // Drop into fault state and confirm we're blinking.
    setAllLatches(false);
    advance(kBootupIgnoreMs + kTickPeriodMs);
    ASSERT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::RED);

    // Clear all faults mid-toggle. Next tick should snap to green.
    setAllLatches(true);
    advance(kTickPeriodMs);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::GREEN);
}

TEST_F(AppTsim, ToggleHalfPeriodHoldsStateBeforeFlipping)
{
    setAllLatches(false);
    advance(kBootupIgnoreMs + kTickPeriodMs);
    ASSERT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::RED);

    // Wait less than one toggle period — should still be RED.
    advance(kToggleMs / 2);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::RED);

    // Cross the toggle boundary — now OFF.
    advance((kToggleMs / 2) + kTickPeriodMs);
    EXPECT_EQ(fakes::tsim::lastColor(), fakes::tsim::Color::OFF);
}
