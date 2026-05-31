#pragma once
#include <cstring>

#include "io_batteryMonitoring_datatypes.hpp"
#include "util_errorCodes.hpp"

namespace io::batteryMonitoring
{
result<void>                    init();
result<float>                   get_voltage_cell(CellReading cell);
result<float>                   get_voltage_system(SystemReading system);
result<float>                   get_current();
result<uint64_t>                get_integrated_charge();
result<float>                   get_temperatureIC();
result<SafetyStatusA>           get_safety_alert_a();
result<SafetyStatusB>           get_safety_alert_b();
result<void>                    send_balancing_subcommand(CellBalance_BitMask cell);
result<void>                    stop_balancing_subcommand();
result<uint16_t>                read_balancing_subcommand();
result<uint16_t>                balacing_time_elapsed();
result<std::array<uint16_t, 5>> get_voltage_OV(uint16_t sub_cmd);
result<std::array<uint16_t, 5>> get_voltage_UV(uint16_t sub_cmd);
void                            alert_handler();
bool                            consume_alert_pending();

result<AlertStatus> read_alarm_status();
result<int16_t>     read_currentcc1();

// result<uint32_t> raw_voltages_and_currents(CellNum cell, Measurement measurement_type);
// result<void>     OTP(void);

} // namespace io::batteryMonitoring
