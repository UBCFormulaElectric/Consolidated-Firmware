#pragma once

#include <cstdint>
#include <expected>
#include <span>

#include "util_errorCodes.hpp"

namespace io::powerMonitoring
{
std::expected<void, ErrorCode> read_register(std::uint16_t reg, std::span<uint8_t> data);
std::expected<void, ErrorCode> write_register(std::uint16_t reg, std::span<const uint8_t> data);

void refresh(void);
bool init(void);
float read_voltage(uint8_t ch);
float read_current(uint8_t ch);
float read_power(uint8_t ch);
} // namespace io::powerMonitoring