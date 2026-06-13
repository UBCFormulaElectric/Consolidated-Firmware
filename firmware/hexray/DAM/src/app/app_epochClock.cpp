#include "app_epochClock.hpp"

#include "io_log.hpp"
#include "io_rtc.hpp"
#include "io_time.hpp"

#include <atomic>

namespace app::epochClock
{

namespace
{
    // STM32 RTC stores year as [0, 99] representing 2000..2099.
    constexpr int RTC_YEAR_BASE = 2000;

    // RTC synchronous prescaler. With LSE @ 32768 Hz and AsynchPrediv=31, the
    // subsecond counter ticks at 1024 Hz, so PREDIV_S = 1023.
    constexpr uint32_t PREDIV_S     = 1023;
    constexpr uint32_t SUBSEC_TICKS = PREDIV_S + 1; // ticks per second (1024)

    // ms conversion constants — only used internally by the day/time split helpers.
    constexpr uint64_t MS_PER_DAY    = 86'400'000ULL;
    constexpr uint64_t MS_PER_HOUR   = 3'600'000ULL;
    constexpr uint64_t MS_PER_MINUTE = 60'000ULL;

    // --- Calendar math (Howard Hinnant's algorithms) -------------------------

    // days_from_civil — days since 1970-01-01 for (y, m, d).
    // y is the full Gregorian year (e.g. 2026), m in [1,12], d in [1,31].
    constexpr int64_t daysFromCivil(int y, unsigned m, unsigned d)
    {
        y -= (m <= 2);
        const int      era = (y >= 0 ? y : y - 399) / 400;
        const unsigned yoe = static_cast<unsigned>(y - era * 400);
        const unsigned doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
        const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
        return static_cast<int64_t>(era) * 146097 + static_cast<int64_t>(doe) - 719468;
    }

    // civil_from_days — (y, m, d) for days since 1970-01-01.
    struct YMD
    {
        int      year;
        unsigned month;
        unsigned day;
    };

    constexpr YMD civilFromDays(int64_t z)
    {
        z += 719468;
        const int64_t  era = (z >= 0 ? z : z - 146096) / 146097;
        const unsigned doe = static_cast<unsigned>(z - era * 146097);
        const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
        const int      y   = static_cast<int>(yoe) + static_cast<int>(era) * 400;
        const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
        const unsigned mp  = (5 * doy + 2) / 153;
        const unsigned d   = doy - (153 * mp + 2) / 5 + 1;
        const unsigned m   = mp < 10 ? mp + 3 : mp - 9;
        return YMD{ y + (m <= 2 ? 1 : 0), m, d };
    }

    // ISO weekday: Mon=1..Sun=7. 1970-01-01 was a Thursday (= 4).
    constexpr uint8_t weekdayFromDays(int64_t days)
    {
        int64_t w = ((days % 7) + 7 + 3) % 7; // shift so Mon=0
        return static_cast<uint8_t>(w + 1);
    }

    // --- Subsecond <-> millisecond conversion --------------------------------

    uint32_t millisecondsFromSubseconds(uint32_t subseconds)
    {
        const uint32_t clamped_subseconds = (subseconds > PREDIV_S) ? PREDIV_S : subseconds;
        const uint32_t ticks_into_second  = PREDIV_S - clamped_subseconds;
        // Round-half-up. Without rounding, integer truncation here plus the
        // matching truncation in timeFromMsOfDay drops up to 1 ms per round-trip
        // for inputs that don't fall on an exact 1024-tick boundary.
        return (ticks_into_second * 1000U + SUBSEC_TICKS / 2U) / SUBSEC_TICKS;
    }

    uint32_t timeToMsOfDay(const io::rtc::Time &t)
    {
        const uint32_t ms_part = millisecondsFromSubseconds(t.subseconds);

        return static_cast<uint32_t>(t.hours) * static_cast<uint32_t>(MS_PER_HOUR) +
               static_cast<uint32_t>(t.minutes) * static_cast<uint32_t>(MS_PER_MINUTE) +
               static_cast<uint32_t>(t.seconds) * static_cast<uint32_t>(MS_PER_SECOND) + ms_part;
    }

    io::rtc::Time timeFromMsOfDay(uint32_t ms_of_day)
    {
        uint32_t remaining = ms_of_day % static_cast<uint32_t>(MS_PER_DAY);

        const uint8_t hours    = static_cast<uint8_t>(remaining / MS_PER_HOUR);
        remaining              = remaining % static_cast<uint32_t>(MS_PER_HOUR);
        const uint8_t minutes  = static_cast<uint8_t>(remaining / MS_PER_MINUTE);
        remaining              = remaining % static_cast<uint32_t>(MS_PER_MINUTE);
        const uint8_t  seconds = static_cast<uint8_t>(remaining / MS_PER_SECOND);
        const uint32_t ms_part = remaining % static_cast<uint32_t>(MS_PER_SECOND);
        // Round-half-up; symmetric with the rounding in millisecondsFromSubseconds
        // so set→get round-trips are lossless for every ms in [0, 999].
        const uint32_t ticks_into_second = (ms_part * SUBSEC_TICKS + 500U) / 1000U;
        const uint32_t subseconds        = PREDIV_S - ticks_into_second;

        return io::rtc::Time(hours, minutes, seconds, subseconds);
    }

    // --- Shared builders -----------------------------------------------------

    struct EpochParts
    {
        int64_t  days;
        uint32_t ms_of_day;
    };

    constexpr EpochParts splitEpoch(uint64_t epoch_ms)
    {
        return { static_cast<int64_t>(epoch_ms / MS_PER_DAY), static_cast<uint32_t>(epoch_ms % MS_PER_DAY) };
    }

    DateTime makeDateTime(int year, unsigned month, unsigned day, const io::rtc::Time &time)
    {
        return DateTime{
            static_cast<uint16_t>(year),
            static_cast<uint8_t>(month),
            static_cast<uint8_t>(day),
            time.hours,
            time.minutes,
            time.seconds,
            static_cast<uint16_t>(millisecondsFromSubseconds(time.subseconds)),
        };
    }

    struct RtcDateTime
    {
        io::rtc::Date date;
        io::rtc::Time time;
    };

    result<RtcDateTime> readRtc()
    {
        // Order matters: HAL requires get_time before get_date to unlock the shadow registers.
        const auto time = io::rtc::get_time();
        if (!time)
            return std::unexpected(time.error());
        const auto date = io::rtc::get_date();
        if (!date)
            return std::unexpected(date.error());
        return RtcDateTime{ *date, *time };
    }

    // --- Fast monotonic-projected clock base ---------------------------------

    // Fast epoch base (epoch ms - monotonic ms). The high-priority CAN RX reader can preempt a
    // lower-priority writer mid 64-bit store, so publish it under a seqlock to avoid torn reads.
    volatile uint32_t base_seq         = 0; // even: stable, odd: update in progress
    volatile uint64_t fast_basetime_ms = 0; // 0 until first anchor

    void setFastBase(const uint64_t epoch_ms)
    {
        const uint64_t base = epoch_ms - io::time::getCurrentMs();
        base_seq            = base_seq + 1U; // mark update in progress (odd)
        std::atomic_signal_fence(std::memory_order_seq_cst);
        fast_basetime_ms = base;
        std::atomic_signal_fence(std::memory_order_seq_cst);
        base_seq = base_seq + 1U; // publish (even)
    }

    // Returns false if a writer is mid-update, the read tore, or no anchor has been taken yet.
    bool readFastBase(uint64_t &base_out)
    {
        const uint32_t seq_before = base_seq;
        std::atomic_signal_fence(std::memory_order_seq_cst);
        const uint64_t base = fast_basetime_ms;
        std::atomic_signal_fence(std::memory_order_seq_cst);
        const uint32_t seq_after = base_seq;
        if (seq_before != seq_after || (seq_before & 1U) != 0U || base == 0)
            return false;
        base_out = base;
        return true;
    }
} // namespace

// --- RTC calendar reads ------------------------------------------------------

result<uint64_t> getEpochMs()
{
    const auto rtc = readRtc();
    if (!rtc)
        return std::unexpected(rtc.error());

    const int      full_year = RTC_YEAR_BASE + static_cast<int>(rtc->date.year);
    const int64_t  days      = daysFromCivil(full_year, rtc->date.month, rtc->date.day);
    const uint64_t ms_of_day = timeToMsOfDay(rtc->time);

    return static_cast<uint64_t>(days) * MS_PER_DAY + ms_of_day;
}

result<DateTime> getDateTime()
{
    const auto rtc = readRtc();
    if (!rtc)
        return std::unexpected(rtc.error());

    return makeDateTime(RTC_YEAR_BASE + static_cast<int>(rtc->date.year), rtc->date.month, rtc->date.day, rtc->time);
}

void logDateTime(const char *prefix)
{
    const auto rtc_datetime = getDateTime();
    if (!rtc_datetime)
    {
        LOG_WARN("%s read failed: %d", prefix, static_cast<int>(rtc_datetime.error()));
        return;
    }

    LOG_INFO(
        "%s: %u-%02u-%02u %02u:%02u:%02u.%03u", prefix, static_cast<unsigned>(rtc_datetime->year),
        static_cast<unsigned>(rtc_datetime->month), static_cast<unsigned>(rtc_datetime->day),
        static_cast<unsigned>(rtc_datetime->hours), static_cast<unsigned>(rtc_datetime->minutes),
        static_cast<unsigned>(rtc_datetime->seconds), static_cast<unsigned>(rtc_datetime->milliseconds));
}

// --- Epoch -> calendar conversions (no RTC read) -----------------------------

result<DateTime> dateTimeFromEpoch(uint64_t epoch_ms)
{
    const auto [days, ms_of_day] = splitEpoch(epoch_ms);
    const YMD ymd                = civilFromDays(days);
    return makeDateTime(ymd.year, ymd.month, ymd.day, timeFromMsOfDay(ms_of_day));
}

result<uint8_t> weekdayFromEpoch(uint64_t epoch_ms)
{
    return weekdayFromDays(splitEpoch(epoch_ms).days);
}

// --- RTC write ---------------------------------------------------------------

result<void> setEpochMs(uint64_t epoch_ms)
{
    const auto [days, ms_of_day] = splitEpoch(epoch_ms);

    const YMD     ymd     = civilFromDays(days);
    const uint8_t weekday = weekdayFromDays(days);

    const io::rtc::Date date(
        weekday, static_cast<uint8_t>(ymd.month), static_cast<uint8_t>(ymd.day),
        static_cast<uint8_t>(ymd.year - RTC_YEAR_BASE));
    const io::rtc::Time time = timeFromMsOfDay(ms_of_day);

    // HAL requires set_time then set_date for the calendar shadow to update consistently.
    auto status = io::rtc::set_time(time);
    if (!status)
        return std::unexpected(status.error());
    status = io::rtc::set_date(date);
    if (!status)
        return std::unexpected(status.error());

    // Re-anchor the fast-clock base to the value we just programmed so getEpochMsFast() picks up the
    // NTP correction instead of continuing from the pre-correction base.
    setFastBase(epoch_ms);
    return {};
}

// --- Fast monotonic-projected clock ------------------------------------------

result<void> anchorBaseTime()
{
    const auto epoch = getEpochMs();
    if (!epoch)
        return std::unexpected(epoch.error());
    setFastBase(*epoch);
    return {};
}

result<uint64_t> getEpochMsFast()
{
    // Project from the base anchored at startup instead of reading the RTC per CAN frame.
    uint64_t base = 0;
    if (!readFastBase(base))
        return getEpochMs(); // unanchored or writer mid-update: fall back to the authoritative RTC
    // Unsigned wrap keeps this correct until the 32-bit monotonic tick wraps (~49 days).
    return base + io::time::getCurrentMs();
}

result<uint64_t> getFastBase()
{
    // Read-only: the base is anchored at startup (jobs_init) and on NTP correction (setEpochMs).
    uint64_t base = 0;
    if (!readFastBase(base))
        return std::unexpected(ErrorCode::ERROR);
    return base;
}

} // namespace app::epochClock
