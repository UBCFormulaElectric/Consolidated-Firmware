#pragma once
#include <stdint.h>
#include <expected>
#include <span>

#include "util_errorCodes.hpp"

namespace io::batteryMonitoring {

std::expected<uint16_t, ErrorCode> get_voltage(std::span<uint8_t> voltage_cell);
std::expected<void, ErrorCode> OTP(void);

} //namespace io::batteryMonitoring
