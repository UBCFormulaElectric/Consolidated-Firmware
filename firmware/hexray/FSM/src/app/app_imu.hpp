#pragma once

#include <expected>
#include "util_errorCodes.hpp"

namespace app::imu
{
struct FSMImuResults
{
    result<float> accel_x_res;
    result<float> accel_y_res;
    result<float> accel_z_res;
    result<float> gyro_x_res;
    result<float> gyro_y_res;
    result<float> gyro_z_res;

    inline constexpr bool hasFault()
    {
        return not(
            accel_x_res.has_value() && accel_y_res.has_value() && accel_z_res.has_value() && gyro_x_res.has_value() &&
            gyro_y_res.has_value() && gyro_z_res.has_value());
    }
};

void init();
void broadcast();
} // namespace app::imu
