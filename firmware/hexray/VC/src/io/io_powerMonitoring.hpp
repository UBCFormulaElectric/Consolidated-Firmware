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

enum Channel : uint8_t
{
    CH1,
    CH2,
    CH3,
    CH4
};
namespace io::powerMonitoring
{
// bitmask: OV (bits 0,1, 2, 3), UV (bits 4, 5, 6, 7)
result<void>    refresh();
result<void>    init();
result<float>   read_voltage(Channel ch);
result<float>   read_current(Channel ch);
result<float>   read_power(Channel ch);
result<uint8_t> read_alert_status();
result<bool>    is_alert_asserted();
result<void>    monitor_power_inputs();
} // namespace io::powerMonitoring
