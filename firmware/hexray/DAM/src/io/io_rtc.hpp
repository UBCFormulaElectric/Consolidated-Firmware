#pragma once

#include <stdint.h>
#include <util_errorCodes.hpp>

namespace io::rtc
{
struct Time
{
    uint8_t hours;       // Must be [0-23]
    uint8_t minutes;     // Must be [0-59]
    uint8_t seconds;     // Must be [0-59]
    uint32_t subseconds; // PREDIV_S is internally set to 999 so this field will be [0, 999 + 1].
                         // ms = (PREDIV_S - subseconds) / (PREDIV_S + 1)
                         // This field is NOT used by HAL_RTC_SetTime().
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
 * @param time Time struct.
 * @return std::expected containing the programmed Time on success,
 *         or an ErrorCode if a HAL error occurred.
 */
std::expected<Time, ErrorCode> set_time(const Time &time);

/**
 * Set the RTC date.
 *
 * @param date Date struct.
 * @return std::expected containing the programmed Date on success,
 *         or an ErrorCode if a HAL error occurred.
 */
std::expected<Date, ErrorCode> set_date(const Date &date);

/**
 * Get the RTC time.
 *
 * @param time Output time.
 * @return std::expected containing the current time on success,
 *         or an ErrorCode if a HAL error occurred.
 */
std::expected<Time, ErrorCode> get_time(Time &time);

/**
 * Get the RTC date.
 *
 * @param date Output date.
 * @return std::expected containing the current date on success,
 *         or an ErrorCode if a HAL error occurred.
 */
std::expected<Date, ErrorCode> get_date(Date &date);

// Helper functions to convert BCD format to binary.
uint8_t bcd_to_bin_8(uint8_t bcd);
uint32_t bcd_to_bin_32(uint32_t bcd);

// Helper function to convert binary to BCD format.
uint8_t bin_to_bcd(uint8_t bin);

} // namespace io::rtc
