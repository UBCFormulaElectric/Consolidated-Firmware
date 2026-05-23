#include "app_epochClock.hpp"

#include <cstdint>
#include <gtest/gtest.h>

// The fake RTC stores whatever Time/Date setEpochMs hands it, so this test
// pins the civil<->epoch conversion in both directions. The STM32 RTC's date
// register only encodes years 2000..2099, so all anchors live in that range.
namespace
{
constexpr uint64_t kMsPerSecond = 1'000ULL;
constexpr uint64_t kMsPerDay    = 86'400'000ULL;

void expectRoundTrip(uint64_t epoch_ms)
{
    ASSERT_TRUE(app::epochClock::setEpochMs(epoch_ms).has_value());
    const auto got = app::epochClock::getEpochMs();
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, epoch_ms);
}
} // namespace

TEST(EpochClock, RoundTripAnchor2000)
{
    // 2000-01-01 00:00:00 UTC — the RTC year origin.
    constexpr uint64_t anchor = 946'684'800'000ULL;
    expectRoundTrip(anchor);
}

TEST(EpochClock, RoundTripLeapDay2024)
{
    // 2024-02-29 12:34:56.789 UTC — exercises both the leap-year branch and
    // sub-second handling.
    constexpr uint64_t leap_day = 1'709'210'096'789ULL;
    expectRoundTrip(leap_day);
}

TEST(EpochClock, RoundTripEndOf2099)
{
    // 2099-12-31 23:59:59.000 UTC — the last second the RTC can represent.
    constexpr uint64_t end_of_2099 = 4'102'444'799'000ULL;
    expectRoundTrip(end_of_2099);
}

TEST(EpochClock, RoundTripWholeSecondBoundaryMidday)
{
    // Some day in 2026 at 12:00:00.000 UTC — whole-second boundary on a day
    // whose ms-of-day is exactly 12 h.
    constexpr uint64_t midday = 1'778'673'600'000ULL;
    EXPECT_EQ(midday % kMsPerDay, 12ULL * 3600ULL * kMsPerSecond);
    expectRoundTrip(midday);
}
