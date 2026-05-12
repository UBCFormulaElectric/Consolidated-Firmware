#pragma once
#include <cstring>

#include "io_batteryMonitoring_datatypes.hpp"
#include "util_errorCodes.hpp"

namespace io::batteryMonitoring
{
std::expected<void, ErrorCode>     init();
std::expected<float, ErrorCode>    get_voltage_cell(CellReading cell);
std::expected<float, ErrorCode>    get_voltage_system(SystemReading system);
std::expected<float, ErrorCode>    get_current();
std::expected<uint64_t, ErrorCode> get_integrated_charge();
std::expected<float, ErrorCode>    get_temperatureIC();
std::expected<void, ErrorCode>     balancing_init();
std::expected<bool, ErrorCode> is_balancing_active();
// std::expected<uint32_t, ErrorCode> raw_voltages_and_currents(CellNum cell, Measurement measurement_type);
// std::expected<void, ErrorCode>     OTP(void);

} // namespace io::batteryMonitoring