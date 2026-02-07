#include "io_rtc.hpp"
#include "io_log.h"

namespace rtc
{
bool set_time(RTC_HandleTypeDef &hrtc, const Time &time)
{
    RTC_TimeTypeDef rtcTime{};

    rtcTime.Hours = bin_to_bcd(time.hours);
    rtcTime.Minutes = bin_to_bcd(time.minutes);
    rtcTime.Seconds = bin_to_bcd(time.seconds);
    rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    rtcTime.StoreOperation = RTC_STOREOPERATION_RESET;

    if (HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD) != HAL_OK)
    {
        LOG_ERROR("Error setting RTC Time");
        return false;
    }
    return true;
}

bool set_date(RTC_HandleTypeDef &hrtc, const Date &date)
{
    RTC_DateTypeDef rtcDate{};

    rtcDate.WeekDay = date.weekday;
    rtcDate.Month = date.month;
    rtcDate.Date = bin_to_bcd(date.day);
    rtcDate.Year = bin_to_bcd(date.year);

    if (HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD) != HAL_OK) 
    {
        LOG_ERROR("Error getting RTC Date");
        return false;
    }
    return true;
}

bool get_time(RTC_HandleTypeDef &hrtc, Time &time)
{
    RTC_TimeTypeDef rtcTime{};
    RTC_DateTypeDef rtcDate{};

    // You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values in the 
    // higher-order calendar shadow registers to ensure consistency between the time and date values.
    if (HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD) != HAL_OK)
        return false;

    if (HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD) != HAL_OK)
        return false;

    time.hours = bcd_to_bin(rtcTime.Hours);
    time.minutes = bcd_to_bin(rtcTime.Minutes);
    time.seconds = bcd_to_bin(rtcTime.Seconds);

    return true;
}

bool get_date(RTC_HandleTypeDef &hrtc, Date &date)
{
    RTC_TimeTypeDef dummy{};
    RTC_DateTypeDef rtcDate{};

    // You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values in the 
    // higher-order calendar shadow registers to ensure consistency between the time and date values.
    if (HAL_RTC_GetTime(&hrtc, &dummy, RTC_FORMAT_BCD) != HAL_OK)
        return false;

    if (HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD) != HAL_OK)
        return false;

    date.weekday = rtcDate.WeekDay;
    date.month = rtcDate.Month;
    date.day = bcd_to_bin(rtcDate.Date);
    date.year = bcd_to_bin(rtcDate.Year);

    return true;
}

static uint8_t bcd_to_bin(uint8_t bcd)
{
    return static_cast<uint8_t>((bcd >> 4) * 10 + (bcd & 0x0F));
}

static uint8_t bin_to_bcd(uint8_t bin)
{
    return static_cast<uint8_t>(((bin / 10) << 4) | (bin % 10));
}
} // namespace rtc
