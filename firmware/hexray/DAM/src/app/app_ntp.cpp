#include "app_ntp.hpp"

#include <cstdint>
#include <cstring>

static constexpr uint32_t PREDIV_S      = 999;
static constexpr uint64_t MS_PER_DAY    = 86400000ULL;
static constexpr uint64_t MS_PER_HOUR   = 3600000ULL;
static constexpr uint64_t MS_PER_MINUTE = 60000ULL;
static constexpr uint64_t MS_PER_SECOND = 1000ULL;

static constexpr uint8_t MIN_NTP_PACKET_SIZE = 17;

namespace app::ntp
{

namespace
{
    Timestamps g_ts{};
}

uint64_t rtcTimeToMs(const io::rtc::Time &t)
{
    const uint32_t clamped_subseconds = (t.subseconds > PREDIV_S) ? PREDIV_S : t.subseconds;
    const uint32_t ms_part            = PREDIV_S - clamped_subseconds;

    return static_cast<uint64_t>(t.hours) * MS_PER_HOUR + static_cast<uint64_t>(t.minutes) * MS_PER_MINUTE +
           static_cast<uint64_t>(t.seconds) * MS_PER_SECOND + static_cast<uint64_t>(ms_part);
}

io::rtc::Time msToRtcTime(const uint64_t &ms)
{
    io::rtc::Time t{};
    uint64_t      remaining_ms = ms % MS_PER_DAY;

    t.hours = static_cast<uint8_t>(remaining_ms / MS_PER_HOUR);
    remaining_ms %= MS_PER_HOUR;

    t.minutes = static_cast<uint8_t>(remaining_ms / MS_PER_MINUTE);
    remaining_ms %= MS_PER_MINUTE;

    t.seconds = static_cast<uint8_t>(remaining_ms / MS_PER_SECOND);

    const uint32_t ms_remainder = static_cast<uint32_t>(remaining_ms % MS_PER_SECOND);
    t.subseconds                = PREDIV_S - ms_remainder;
    return t;
}

int64_t computeOffset(const Timestamps &ts)
{
    return ((int64_t)(ts.t1 - ts.t0) + (int64_t)(ts.t2 - ts.t3)) / 2;
}

bool parseNTPPacketBody(std::span<const uint8_t> body, Timestamps &ts)
{
    if (body.size() < MIN_NTP_PACKET_SIZE)
        return false;

    // Wire format is little endian
    uint64_t t1;
    uint64_t t2;
    std::memcpy(&t1, &body[1], sizeof(uint64_t));
    std::memcpy(&t2, &body[9], sizeof(uint64_t));

    ts.t1 = t1 % MS_PER_DAY;
    ts.t2 = t2 % MS_PER_DAY;
    return true;
}

void recordT0(uint64_t t0_ms)
{
    g_ts.t0 = t0_ms;
}

const Timestamps &timestamps()
{
    return g_ts;
}

std::optional<uint64_t> handleFrame(std::span<const uint8_t> body, uint64_t t3_ms, uint64_t current_rtc_ms)
{
    if (!parseNTPPacketBody(body, g_ts))
        return std::nullopt;

    g_ts.t3 = t3_ms;

    const int64_t theta  = computeOffset(g_ts);
    int64_t       new_ms = static_cast<int64_t>(current_rtc_ms) + theta;
    if (new_ms < 0)
        new_ms = 0;
    return static_cast<uint64_t>(new_ms);
}

} // namespace app::ntp
