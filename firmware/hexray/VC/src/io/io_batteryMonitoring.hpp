#pragma once
#include <stdint.h>
#include <expected>
#include <span>

#include "io_batteryMonitoring_datatypes.hpp"
#include "util_errorCodes.hpp"

namespace io::batteryMonitoring
{

std::expected<void, ErrorCode>     init(void);
std::expected<void, ErrorCode>     tick(void);
std::expected<void, ErrorCode>     random(void);
std::expected<uint16_t, ErrorCode> get_voltage_cell(CellReading cell);
std::expected<uint16_t, ErrorCode> get_voltage_system(SystemReading system);
// std::expected<uint32_t, ErrorCode> raw_voltages_and_currents(CellNum cell, Measurement measurement_type);
// std::expected<void, ErrorCode>     OTP(void);

} // namespace io::batteryMonitoring