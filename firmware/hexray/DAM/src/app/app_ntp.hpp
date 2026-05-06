#pragma once

#include <cstdint>
#include <optional>
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

uint64_t      rtcTimeToMs(const io::rtc::Time &t);
io::rtc::Time msToRtcTime(const uint64_t &ms);
int64_t       computeOffset(const Timestamps &ts);
bool          parseNTPPacketBody(std::span<const uint8_t> body, Timestamps &ts);

// Stash t0 (TX-side timestamp) so the RX-side handler can pair it with the
// t1/t2 carried in the response and the t3 captured at receive.
void recordT0(uint64_t t0_ms);

// Read the most recently recorded timestamps. Used by tests and diagnostics.
const Timestamps &timestamps();

// parse into g_ts and compute the new ms time based on the current RTC time
std::optional<uint64_t> handleFrame(std::span<const uint8_t> body, uint64_t t3_ms, uint64_t current_rtc_ms);

// Parse an NTP frame and apply the resulting correction directly to the RTC.
// Returns false if any stage fails.
bool handleFrameAndTuneRtc(std::span<const uint8_t> body, uint64_t t3_ms);

} // namespace app::ntp
