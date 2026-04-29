#include "io_rtc.hpp"

namespace io::rtc
{

// Non-static so the test-only `fakes::rtc` helpers below can reach them.
// Only this TU defines io::rtc symbols in the test build, so no link conflict.
Time g_time{};
Time g_last_set_time{};
bool g_last_set_called = false;

std::expected<void, ErrorCode> set_time(const Time &time)
{
    g_last_set_time   = time;
    g_last_set_called = true;
    g_time            = time;
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

// Test-only spies/seeders. Kept out of io::rtc so production code can never
// accidentally call them.
namespace fakes::rtc
{
void setNow(const io::rtc::Time &t)
{
    io::rtc::g_time = t;
}

io::rtc::Time lastSetTime()
{
    return io::rtc::g_last_set_time;
}

bool wasSetCalled()
{
    return io::rtc::g_last_set_called;
}

void reset()
{
    io::rtc::g_time            = {};
    io::rtc::g_last_set_time   = {};
    io::rtc::g_last_set_called = false;
}
} // namespace fakes::rtc
