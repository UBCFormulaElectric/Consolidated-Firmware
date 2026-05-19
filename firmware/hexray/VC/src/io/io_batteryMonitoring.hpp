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
std::expected<SafetyStatusA, ErrorCode> get_safety_alert_a();
std::expected<SafetyStatusB, ErrorCode> get_safety_alert_b();
std::expected<bool, ErrorCode> is_cell_ov_uv_fault_active();
std::expected<void, ErrorCode> send_balancing_subcommand(CellBalance_BitMask cell);
std::expected<void, ErrorCode> stop_balancing_subcommand();
std::expected<uint16_t, ErrorCode> read_balancing_subcommand();
std::expected<uint16_t, ErrorCode> read_balacing_time_seconds();
void alert_handler();
bool is_alert_pending();
void clear_alert_pending();

std::expected<void, ErrorCode> get_Ttemperature();
std::expected<AlertStatus, ErrorCode> read_alarm_status();
std::expected<int16_t, ErrorCode> read_currentcc1();

// std::expected<uint32_t, ErrorCode> raw_voltages_and_currents(CellNum cell, Measurement measurement_type);
// std::expected<void, ErrorCode>     OTP(void);

} // namespace io::batteryMonitoring
