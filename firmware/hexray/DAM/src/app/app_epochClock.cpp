#include "app_epochClock.hpp"

#include "io_rtc.hpp"

namespace app::epochClock
{

namespace
{
    // STM32 RTC stores year as [0, 99] representing 2000..2099.
    constexpr int RTC_YEAR_BASE = 2000;

    // RTC subseconds prescaler — STM32 HAL hardware detail, kept private.
    constexpr uint32_t PREDIV_S = 999;

    // ms conversion constants — only used internally by the day/time split helpers.
    constexpr uint64_t MS_PER_DAY    = 86'400'000ULL;
    constexpr uint64_t MS_PER_HOUR   = 3'600'000ULL;
    constexpr uint64_t MS_PER_MINUTE = 60'000ULL;

    // Howard Hinnant's days_from_civil — days since 1970-01-01 for (y, m, d).
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

    // Howard Hinnant's civil_from_days — (y, m, d) for days since 1970-01-01.
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
        return { y + (m <= 2 ? 1 : 0), m, d };
    }

    // ISO weekday: Mon=1..Sun=7. 1970-01-01 was a Thursday (= 4).
    constexpr uint8_t weekdayFromDays(int64_t days)
    {
        int64_t w = ((days % 7) + 7 + 3) % 7; // shift so Mon=0
        return static_cast<uint8_t>(w + 1);
    }

    uint32_t timeToMsOfDay(const io::rtc::Time &t)
    {
        const uint32_t clamped_subseconds = (t.subseconds > PREDIV_S) ? PREDIV_S : t.subseconds;
        const uint32_t ms_part            = PREDIV_S - clamped_subseconds;

        return static_cast<uint32_t>(t.hours) * static_cast<uint32_t>(MS_PER_HOUR) +
               static_cast<uint32_t>(t.minutes) * static_cast<uint32_t>(MS_PER_MINUTE) +
               static_cast<uint32_t>(t.seconds) * static_cast<uint32_t>(MS_PER_SECOND) + ms_part;
    }

    io::rtc::Time timeFromMsOfDay(uint32_t ms_of_day)
    {
        uint32_t remaining = ms_of_day % static_cast<uint32_t>(MS_PER_DAY);

        const uint8_t hours       = static_cast<uint8_t>(remaining / MS_PER_HOUR);
        remaining                 = remaining % static_cast<uint32_t>(MS_PER_HOUR);
        const uint8_t minutes     = static_cast<uint8_t>(remaining / MS_PER_MINUTE);
        remaining                 = remaining % static_cast<uint32_t>(MS_PER_MINUTE);
        const uint8_t  seconds    = static_cast<uint8_t>(remaining / MS_PER_SECOND);
        const uint32_t ms_part    = remaining % static_cast<uint32_t>(MS_PER_SECOND);
        const uint32_t subseconds = PREDIV_S - ms_part;

        return io::rtc::Time(hours, minutes, seconds, subseconds);
    }
} // namespace

std::expected<uint64_t, ErrorCode> getEpochMs()
{
    // Order matters: HAL requires get_time before get_date to unlock the
    // shadow registers.
    const auto time = io::rtc::get_time();
    if (!time)
        return std::unexpected(time.error());
    const auto date = io::rtc::get_date();
    if (!date)
        return std::unexpected(date.error());

    const int      full_year = RTC_YEAR_BASE + static_cast<int>(date->year);
    const int64_t  days      = daysFromCivil(full_year, date->month, date->day);
    const uint64_t ms_of_day = timeToMsOfDay(*time);

    return static_cast<uint64_t>(days) * MS_PER_DAY + ms_of_day;
}

std::expected<void, ErrorCode> setEpochMs(uint64_t epoch_ms)
{
    const int64_t  days      = static_cast<int64_t>(epoch_ms / MS_PER_DAY);
    const uint32_t ms_of_day = static_cast<uint32_t>(epoch_ms % MS_PER_DAY);

    const YMD     ymd     = civilFromDays(days);
    const uint8_t weekday = weekdayFromDays(days);

    const io::rtc::Date date(
        weekday, static_cast<uint8_t>(ymd.month), static_cast<uint8_t>(ymd.day),
        static_cast<uint8_t>(ymd.year - RTC_YEAR_BASE));
    const io::rtc::Time time = timeFromMsOfDay(ms_of_day);

    // HAL requires set_time then set_date for the calendar shadow to update
    // consistently.
    auto status = io::rtc::set_time(time);
    if (!status)
        return std::unexpected(status.error());
    status = io::rtc::set_date(date);
    if (!status)
        return std::unexpected(status.error());
    return {};
}

} // namespace app::epochClock
