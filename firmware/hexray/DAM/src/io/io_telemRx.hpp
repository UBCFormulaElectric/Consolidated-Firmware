#pragma once
#include <cstdint>
#include <expected>
#include <span>
#include <util_errorCodes.hpp>

namespace io::telemRx
{
// RX side: block on the radio UART, fill `scratch`, and return a span over the
// filled portion. Caller owns `scratch` and forwards the result to whatever
// consumer wants it (no app coupling here).
std::expected<std::span<const uint8_t>, ErrorCode> read(std::span<uint8_t> scratch);
} // namespace io::telemRx
