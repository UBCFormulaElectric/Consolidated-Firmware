#include "app_ntp.hpp"

#include <cstring>

namespace app::ntp
{

uint64_t rtcTimeToMs(io::rtc::Time t)
{
    uint32_t ms_part = PREDIV_S - t.subseconds;
    return static_cast<uint64_t>(t.hours) * 3600000ULL + static_cast<uint64_t>(t.minutes) * 60000ULL +
           static_cast<uint64_t>(t.seconds) * 1000ULL + static_cast<uint64_t>(ms_part);
}

io::rtc::Time msToRtcTime(uint64_t ms)
{
    io::rtc::Time t{};

    ms %= MS_PER_DAY;
    t.hours = static_cast<uint8_t>(ms / 3600000ULL);
    ms %= 3600000ULL;
    t.minutes = static_cast<uint8_t>(ms / 60000ULL);
    ms %= 60000ULL;

    t.seconds             = static_cast<uint8_t>(ms / 1000ULL);
    uint32_t ms_remainder = static_cast<uint32_t>(ms % 1000ULL);

    t.subseconds = PREDIV_S - ms_remainder;
    return t;
}

int64_t computeOffset(const Timestamps &ts)
{
    return ((int64_t)(ts.t1 - ts.t0) + (int64_t)(ts.t2 - ts.t3)) / 2;
}

bool parseNTPPacketBody(std::span<uint8_t> body, Timestamps &ts)
{
    if (body.size() < 17)
        return false;

    uint64_t messageID = body[0];
    if (messageID != 1)
        return false;

    uint64_t t1;
    uint64_t t2;

    std::memcpy(&t1, &body[1], sizeof(uint64_t));
    std::memcpy(&t2, &body[9], sizeof(uint64_t));

    ts.t1 = t1 % MS_PER_DAY;
    ts.t2 = t2 % MS_PER_DAY;
    return true;
}

} // namespace app::ntp
