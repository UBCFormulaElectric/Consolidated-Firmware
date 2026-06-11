#pragma once
#include <cstdint>
#include <expected>
#include <span>
#include "util_errorCodes.hpp"

union AlertOvUvBits
{
    struct __attribute__((packed))
    {
        uint32_t CH4OV : 1;
        uint32_t CH3OV : 1;
        uint32_t CH2OV : 1;
        uint32_t CH1OV : 1;
        uint32_t CH4UV : 1;
        uint32_t CH3UV : 1;
        uint32_t CH2UV : 1;
        uint32_t CH1UV : 1;
    } bits;
    uint8_t alert_status;
#ifdef TARGET_EMBEDDED
    static_assert(sizeof(bits) == sizeof(alert_status));
#endif
};

/**
 * NOTE: CH1 is meant to represent PCM and CH4 is meant to represent BOOST, however the nets were swapped on
 * schematic, so I am js gonna swap in app layer
 */

enum Channel : uint8_t
{
    CH1 = 1,
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
