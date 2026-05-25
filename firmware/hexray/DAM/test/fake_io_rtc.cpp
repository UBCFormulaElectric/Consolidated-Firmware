#include "io_rtc.hpp"

#include "app_epochClock.hpp"

namespace io::rtc
{

// Non-static so the test-only `fakes::rtc` helpers below can reach them.
// Only this TU defines io::rtc symbols in the test build, so no link conflict.
Time g_time{ 0, 0, 0, 1023 };
Date g_date{ 4, 1, 1, 0 }; // 2000-01-01, Thursday (weekday=4 in ISO with Mon=1 doesn't fit; fake doesn't care)
Time g_last_set_time{ 0, 0, 0, 1023 };
Date g_last_set_date{ 4, 1, 1, 0 };
bool g_last_set_called = false;

result<void> set_time(const Time &time)
{
    g_last_set_time   = time;
    g_last_set_called = true;
    g_time            = time;
    return {};
}

result<void> set_date(const Date &date)
{
    g_last_set_date = date;
    g_date          = date;
    return {};
}

result<Time> get_time()
{
    return g_time;
}

result<Date> get_date()
{
    return g_date;
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

void setEpochMs(uint64_t epoch_ms)
{
    // Drive the same conversion path that production setEpochMs uses, but
    // bypass the spy flags so seeding doesn't look like a production write.
    auto res = app::epochClock::setEpochMs(epoch_ms);
    (void)res;
    io::rtc::g_last_set_called = false;
}

io::rtc::Time lastSetTime()
{
    return io::rtc::g_last_set_time;
}

io::rtc::Date lastSetDate()
{
    return io::rtc::g_last_set_date;
}

bool wasSetCalled()
{
    return io::rtc::g_last_set_called;
}

void reset()
{
    io::rtc::g_time            = io::rtc::Time(0, 0, 0, 1023);
    io::rtc::g_date            = io::rtc::Date(4, 1, 1, 0);
    io::rtc::g_last_set_time   = io::rtc::Time(0, 0, 0, 1023);
    io::rtc::g_last_set_date   = io::rtc::Date(4, 1, 1, 0);
    io::rtc::g_last_set_called = false;
}
} // namespace fakes::rtc
