#pragma once

#include <stdint.h>

namespace io::rtc
{
struct Time
{
    uint8_t hours;   // Must be [0-23]
    uint8_t minutes; // Must be [0-59]
    uint8_t seconds; // Must be [0-59]
};

struct Date
{
    uint8_t weekday; // Must be [1-7]. 1 = Monday, 7 = Sunday
    uint8_t month;   // Must be [1-12]
    uint8_t day;     // Must be [1-31]
    uint8_t year;    // Must be [0-99]
};

/**
 * Set the RTC time.
 *
 * @param hrtc RTC HAL handle.
 * @param time Time struct.
 * @return true on success, false on HAL error.
 */
bool set_time(const Time &time);

/**
 * Set the RTC date.
 *
 * @param hrtc RTC HAL handle.
 * @param date Date struct.
 * @return true on success, false on HAL error.
 */
bool set_date(const Date &date);

/**
 * Get the RTC time.
 *
 * @param hrtc RTC HAL handle.
 * @param time Output time.
 * @return true on success, false on HAL error.
 */
bool get_time(Time &time);

/**
 * Get the RTC date.
 *
 * @param hrtc RTC HAL handle.
 * @param date Output date.
 * @return true on success, false on HAL error.
 */
bool get_date(Date &date);

// Helper function to convert BCD format to binary.
uint8_t bcd_to_bin(uint8_t bcd);

// Helper function to convert binary to BCD format.
uint8_t bin_to_bcd(uint8_t bin);

} // namespace io::rtc
