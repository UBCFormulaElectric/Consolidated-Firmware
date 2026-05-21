#pragma once

#include <stdint.h>
#include <util_errorCodes.hpp>

namespace io::rtc
{
struct Time
{
    uint8_t  hours;      // Must be [0-23]
    uint8_t  minutes;    // Must be [0-59]
    uint8_t  seconds;    // Must be [0-59]
    uint32_t subseconds; // PREDIV_S is internally set to 999 so this field will be [0, 999 + 1].
                         // ms = PREDIV_S - subseconds
                         // This field is NOT used by HAL_RTC_SetTime().

    Time() = delete;
    constexpr Time(uint8_t h, uint8_t m, uint8_t s, uint32_t ss) : hours(h), minutes(m), seconds(s), subseconds(ss) {}
};

struct Date
{
    uint8_t weekday; // Must be [1-7]. 1 = Monday, 7 = Sunday
    uint8_t month;   // Must be [1-12]
    uint8_t day;     // Must be [1-31]
    uint8_t year;    // Must be [0-99], representing 2000-2099.

    Date() = delete;
    constexpr Date(uint8_t w, uint8_t mo, uint8_t d, uint8_t y) : weekday(w), month(mo), day(d), year(y) {}
};

/**
 * Set the RTC time.
 */
result<void> set_time(const Time &time);

/**
 * Set the RTC date.
 */
result<void> set_date(const Date &date);

/**
 * Get the RTC time. Reading time also unlocks the date shadow register, so
 * callers that need both should still read date after this.
 */
result<Time> get_time();

/**
 * Get the RTC date. Must be called after get_time() if the caller also reads
 * time, to keep the calendar shadow consistent.
 */
result<Date> get_date();

// Helper functions to convert BCD format to binary.
uint8_t bcd_to_bin(uint8_t bcd);

// Helper function to convert binary to BCD format.
uint8_t bin_to_bcd(uint8_t bin);

} // namespace io::rtc
