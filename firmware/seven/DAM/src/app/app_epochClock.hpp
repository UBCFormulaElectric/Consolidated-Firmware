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
result<uint64_t> getEpochMs();

// Anchor the fast-clock base to the current RTC time. Call once at startup, after the RTC is known
// good and before CAN traffic flows. setEpochMs() re-anchors automatically on an NTP correction.
result<void> anchorBaseTime();

// Cheap epoch ms for hot paths: projects from the anchored base using the monotonic clock (no
// per-call RTC read). Use this on the CAN RX/telem path instead of getEpochMs().
result<uint64_t> getEpochMsFast();

// The anchored fast-clock base (epoch ms at boot) that getEpochMsFast() projects from. Use this for
// the on-disk log basetime instead of getEpochMsFast() - getCurrentMs(), which carries a small skew.
result<uint64_t> getFastBase();

// Split Unix epoch ms into {Date, Time} and program both into the RTC.
result<void> setEpochMs(uint64_t epoch_ms);

// Read RTC date/time into a printable calendar representation.
result<DateTime> getDateTime();

// Convert an absolute Unix epoch milliseconds value into a calendar DateTime
// (UTC) without reading the RTC. Useful for converting stored epoch values
// into human calendar fields.
result<DateTime> dateTimeFromEpoch(uint64_t epoch_ms);

// Get ISO weekday (Mon=1..Sun=7) for an epoch ms value without reading RTC.
result<uint8_t> weekdayFromEpoch(uint64_t epoch_ms);

// Log current RTC date/time as YYYY-MM-DD HH:MM:SS.mmm.
void logDateTime(const char *prefix);

} // namespace app::epochClock
