#include "io_rtc.hpp"

namespace io::rtc
{

static Time g_time{};

std::expected<void, ErrorCode> set_time(const Time &time)
{
    g_time = time;
    return {};
}

std::expected<void, ErrorCode> set_date(const Date &)
{
    return {};
}

std::expected<Time, ErrorCode> get_time(Time &time)
{
    time = g_time;
    return g_time;
}

std::expected<Date, ErrorCode> get_date(Date &date)
{
    return date;
}

uint8_t bcd_to_bin(uint8_t bcd)
{
    return bcd;
}

uint8_t bin_to_bcd(uint8_t bin)
{
    return bin;
}

} // namespace io::rtc
