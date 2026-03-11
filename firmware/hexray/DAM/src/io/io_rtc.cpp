#include "io_rtc.hpp"
#include "io_log.h"
#include "hw_utils.hpp"
#include "main.h"

namespace io::rtc
{
std::expected<void, ErrorCode> set_time(const Time &time)
{
    RTC_TimeTypeDef rtcTime{};
    rtcTime.Hours   = bin_to_bcd(time.hours);
    rtcTime.Minutes = bin_to_bcd(time.minutes);
    rtcTime.Seconds = bin_to_bcd(time.seconds);
    // The subseconds field is not used by HAL_RTC_SetTime().
    rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    rtcTime.StoreOperation = RTC_STOREOPERATION_RESET;

    std::expected<void, ErrorCode> status = hw_utils_convertHalStatus(HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD));
    if (!status)
    {
        LOG_ERROR("Error setting RTC Time");
        return std::unexpected(status.error());
    }
    return {};
}

std::expected<void, ErrorCode> set_date(const Date &date)
{
    RTC_DateTypeDef rtcDate{};
    rtcDate.WeekDay = date.weekday;
    rtcDate.Month   = date.month;
    rtcDate.Date    = bin_to_bcd(date.day);
    rtcDate.Year    = bin_to_bcd(date.year);

    std::expected<void, ErrorCode> status = hw_utils_convertHalStatus(HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD));
    if (!status)
    {
        LOG_ERROR("Error setting RTC Date");
        return std::unexpected(status.error());
    }
    return {};
}

std::expected<Time, ErrorCode> get_time(Time &time)
{
    RTC_TimeTypeDef rtcTime{};
    RTC_DateTypeDef rtcDate{};

    // You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values in the
    // higher-order calendar shadow registers to ensure consistency between the time and date values.
    std::expected<void, ErrorCode> status = hw_utils_convertHalStatus(HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD));
    if (!status)
        return std::unexpected(status.error());

    status = hw_utils_convertHalStatus(HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD));
    if (!status)
        return std::unexpected(status.error());

    time.hours      = bcd_to_bin_8(rtcTime.Hours);
    time.minutes    = bcd_to_bin_8(rtcTime.Minutes);
    time.seconds    = bcd_to_bin_8(rtcTime.Seconds);
    time.subseconds = bcd_to_bin_32(rtcTime.SubSeconds);

    return time;
}

std::expected<Date, ErrorCode> get_date(Date &date)
{
    RTC_TimeTypeDef dummy{};
    RTC_DateTypeDef rtcDate{};

    // You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values in the
    // higher-order calendar shadow registers to ensure consistency between the time and date values.
    std::expected<void, ErrorCode> status = hw_utils_convertHalStatus(HAL_RTC_GetTime(&hrtc, &dummy, RTC_FORMAT_BCD));
    if (!status)
        return std::unexpected(status.error());

    status = hw_utils_convertHalStatus(HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD));
    if (!status)
        return std::unexpected(status.error());

    date.weekday = rtcDate.WeekDay;
    date.month   = rtcDate.Month;
    date.day     = bcd_to_bin_8(rtcDate.Date);
    date.year    = bcd_to_bin_8(rtcDate.Year);

    return date;
}

uint8_t bcd_to_bin_8(uint8_t bcd)
{
    return static_cast<uint8_t>((bcd >> 4) * 10 + (bcd & 0x0F));
}

uint32_t bcd_to_bin_32(uint32_t bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t bin_to_bcd(uint8_t bin)
{
    return static_cast<uint8_t>(((bin / 10) << 4) | (bin % 10));
}
} // namespace io::rtc
