#pragma once
#include <cstdint>
#include <expected>
#include <span>
#include "util_errorCodes.hpp"

enum AlertOvUvBits : uint8_t
{
    ALERT_OV_CH1 = (1u << 0),
    ALERT_OV_CH2 = (1u << 1),
    ALERT_OV_CH3 = (1u << 2),
    ALERT_OV_CH4 = (1u << 3),
    ALERT_UV_CH1 = (1u << 4),
    ALERT_UV_CH2 = (1u << 5),
    ALERT_UV_CH3 = (1u << 6),
    ALERT_UV_CH4 = (1u << 7),
};
namespace io::powerMonitoring
{
// bitmask: OV (bits 0,1, 2, 3), UV (bits 4, 5, 6, 7)
std::expected<void, ErrorCode>    refresh();
std::expected<void, ErrorCode>    init();
std::expected<float, ErrorCode>   read_voltage(uint8_t ch);
std::expected<float, ErrorCode>   read_current(uint8_t ch);
std::expected<float, ErrorCode>   read_power(uint8_t ch);
std::expected<uint8_t, ErrorCode> read_alert_status();
std::expected<bool, ErrorCode>    is_alert_asserted();
std::expected<void, ErrorCode>    monitor_power_inputs();
} // namespace io::powerMonitoring
