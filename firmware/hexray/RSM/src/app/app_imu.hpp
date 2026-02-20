#pragma once

#include <expected>
#include "util_errorCodes.hpp"

namespace app::imu
{
struct RSMImuResults {
    std::expected<float, ErrorCode> accel_x_res;
    std::expected<float, ErrorCode> accel_y_res;
    std::expected<float, ErrorCode> accel_z_res;
    std::expected<float, ErrorCode> gyro_x_res;
    std::expected<float, ErrorCode> gyro_y_res;
    std::expected<float, ErrorCode> gyro_z_res;

    inline constexpr bool hasFault()
    {
        return not(
            accel_x_res.has_value() && accel_y_res.has_value() && accel_z_res.has_value() &&
            gyro_x_res.has_value() && gyro_y_res.has_value() && gyro_z_res.has_value());
    }
};

void init();
void broadcast();
} // namespace app::imu
