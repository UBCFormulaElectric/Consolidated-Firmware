#pragma once

#include <span>

#include "io_rtc.hpp"

namespace app::ntp
{

struct Timestamps
{
    uint64_t t0;
    uint64_t t1;
    uint64_t t2;
    uint64_t t3;
};

uint64_t      rtcTimeToMs(io::rtc::Time t);
io::rtc::Time msToRtcTime(uint64_t ms);
int64_t       computeOffset(const Timestamps &ts);
bool          parseNTPPacketBody(std::span<uint8_t> body, Timestamps &ts);

} // namespace app::ntp
