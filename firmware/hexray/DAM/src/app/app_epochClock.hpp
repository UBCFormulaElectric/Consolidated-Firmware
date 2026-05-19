#pragma once

#include <cstdint>
#include <expected>
#include <util_errorCodes.hpp>

namespace app::epochClock
{

inline constexpr uint64_t MS_PER_SECOND = 1'000ULL;

struct DateTime
{
    uint16_t year; // Full year, e.g. 2026.
    uint8_t  month;
    uint8_t  day;
    uint8_t  hours;
    uint8_t  minutes;
    uint8_t  seconds;
    uint16_t milliseconds;
};

// Read both Date and Time from the RTC and return Unix epoch ms.
std::expected<uint64_t, ErrorCode> getEpochMs();

// Split Unix epoch ms into {Date, Time} and program both into the RTC.
std::expected<void, ErrorCode> setEpochMs(uint64_t epoch_ms);

// Read RTC date/time into a printable calendar representation.
std::expected<DateTime, ErrorCode> getDateTime();

// Log current RTC date/time as YYYY-MM-DD HH:MM:SS.mmm.
void logDateTime(const char *prefix);

} // namespace app::epochClock
