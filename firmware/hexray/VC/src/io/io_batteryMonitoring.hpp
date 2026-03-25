#pragma once
#include <stdint.h>
#include <expected>
#include <span>

#include "util_errorCodes.hpp"

namespace io::batteryMonitoring {

std::expected<void, ErrorCode> read_cell_voltage(void);
uint16_t calculate_pack_voltage(void);
std::expected<void, ErrorCode> write_command_buffer(uint16_t sub_cmd, std::span<const uint8_t> data);
std::expected<void, ErrorCode> read_command_buffer(uint16_t sub_cmd, std::span<uint8_t> data);

} //namespace io::batteryMonitoring
