#pragma once
#include <cstdint>
#include <expected>
#include <span>
#include "util_errorCodes.hpp"

namespace io::powerMonitoring
{
std::expected<void, ErrorCode>    refresh(void);
std::expected<void, ErrorCode>    init(void);
std::expected<float, ErrorCode>   read_voltage(uint8_t ch);
std::expected<float, ErrorCode>   read_current(uint8_t ch);
std::expected<float, ErrorCode>   read_power(uint8_t ch);
std::expected<uint8_t, ErrorCode> read_alert_status();
bool                              is_alert_asserted();
} // namespace io::powerMonitoring
