#pragma once
#include <cstdint>
#include <expected>
#include <span>
#include <util_errorCodes.hpp>

#include "io_rtc.hpp"

namespace io::telemRx
{
struct RxChunk
{
    std::span<const uint8_t> bytes;
    io::rtc::Time            rx_time; // captured immediately after the chunk landed
};

// TX side: transmit the NTP start message and return the RTC time captured
// just before transmission (this is the t0 the APP layer needs).
std::expected<io::rtc::Time, ErrorCode> transmitNTPStartMsg();

// RX side: block on the radio UART, fill `scratch`, and return the chunk +
// receive timestamp. Caller owns `scratch` and forwards the result to whatever
// consumer wants it (no app coupling here).
std::expected<RxChunk, ErrorCode> read(std::span<uint8_t> scratch);
} // namespace io::telemRx
