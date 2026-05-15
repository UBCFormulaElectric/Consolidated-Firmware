#include "app_ntp.hpp"

#include <atomic>

#include "app_epochClock.hpp"
#include "io_log.hpp"
#include "io_time.hpp"

static constexpr uint8_t RTC_SET_MAX_ATTEMPTS = 3;
static constexpr uint8_t RTC_GET_MAX_ATTEMPTS = 3;

namespace app::ntp
{

namespace
{
    Timestamps        g_ts{};
    std::atomic<bool> g_ntp_in_progress{ false };

    int64_t computeOffset(const Timestamps &ts)
    {
        return ((int64_t)(ts.t1 - ts.t0) + (int64_t)(ts.t2 - ts.t3)) / 2;
    }

    void recordT0(const uint64_t t0_ms)
    {
        g_ts.t0 = t0_ms;
    }

    uint64_t handleFrame(const uint64_t t1, const uint64_t t2, const uint64_t t3_ms, const uint64_t current_rtc_ms)
    {
        g_ts.t1 = t1;
        g_ts.t2 = t2;
        g_ts.t3 = t3_ms;

        const int64_t theta  = computeOffset(g_ts);
        int64_t       new_ms = static_cast<int64_t>(current_rtc_ms) + theta;
        if (new_ms < 0)
            new_ms = 0;
        return static_cast<uint64_t>(new_ms);
    }
} // namespace

#ifdef TARGET_TEST
const Timestamps &timestamps()
{
    return g_ts;
}
#endif

bool handleFrameAndTuneRtc(const uint64_t t1, const uint64_t t2, const uint64_t t3_ms)
{
    std::expected<uint64_t, ErrorCode> current_epoch_ms = std::unexpected(ErrorCode::ERROR);
    for (uint8_t attempt = 0; attempt < RTC_GET_MAX_ATTEMPTS; ++attempt)
    {
        current_epoch_ms = app::epochClock::getEpochMs();
        if (current_epoch_ms)
            break;
    }
    if (!current_epoch_ms)
    {
        LOG_ERROR("ntp: RTC getEpochMs failed");
        g_ntp_in_progress.store(false);
        return false;
    }

    const uint64_t new_ms = handleFrame(t1, t2, t3_ms, current_epoch_ms.value());

    // Sleep out the sub-second remainder so the RTC write lands on a whole-
    // second boundary.
    const uint32_t remainder_ms = static_cast<uint32_t>(new_ms % app::epochClock::MS_PER_SECOND);
    const uint32_t wait_ms =
        (remainder_ms == 0) ? 0U : static_cast<uint32_t>(app::epochClock::MS_PER_SECOND) - remainder_ms;
    if (wait_ms > 0)
    {
        io::time::delay(wait_ms);
    }

    const uint64_t tuned_epoch_ms = new_ms + wait_ms;
    bool           set_ok         = false;
    for (uint8_t attempt = 0; attempt < RTC_SET_MAX_ATTEMPTS; ++attempt)
    {
        if (app::epochClock::setEpochMs(tuned_epoch_ms))
        {
            set_ok = true;
            break;
        }
    }

    if (!set_ok)
    {
        LOG_ERROR("ntp: RTC setEpochMs failed");
        g_ntp_in_progress.store(false);
        return false;
    }

    // SEGGER_RTT_printf only supports 32-bit %u (no %llu), so print as two
    // 32-bit halves to avoid the printf eating the wrong bytes off varargs.
    LOG_INFO(
        "Tuned RTC! New epoch ms: 0x%08x%08x", static_cast<uint32_t>(tuned_epoch_ms >> 32),
        static_cast<uint32_t>(tuned_epoch_ms & 0xFFFFFFFFu));

    // Log the current RTC time (epoch ms) after tuning
    LOG_INFO(
        "Current RTC time (epoch ms): 0x%08x%08x",
        static_cast<uint32_t>((app::epochClock::getEpochMs().value_or(0)) >> 32),
        static_cast<uint32_t>(app::epochClock::getEpochMs().value_or(0) & 0xFFFFFFFFu));
    g_ntp_in_progress.store(false);
    return true;
}

bool tryBeginAndCaptureT0()
{
    bool expected = false;
    if (!g_ntp_in_progress.compare_exchange_strong(expected, true))
    {
        // LOG_WARN("NTP in progress flag is true, expected: %d", expected);
        return false;
    }
    for (uint8_t attempt = 0; attempt < RTC_GET_MAX_ATTEMPTS; ++attempt)
    {
        const auto t0 = app::epochClock::getEpochMs();
        if (t0)
        {
            recordT0(*t0);
            return true;
        }
    }
    LOG_ERROR("ntp: RTC getEpochMs failed capturing T0");
    g_ntp_in_progress.store(false);
    return false;
}

} // namespace app::ntp
