#pragma once

// refactor needed
#include <expected>
#include "io_imus.hpp"

namespace app::imus
{
struct VCImuResults
{
    std::expected<float, ErrorCode> accel_x_res;
    std::expected<float, ErrorCode> accel_y_res;
    std::expected<float, ErrorCode> accel_z_res;
    std::expected<float, ErrorCode> gyro_x_res;
    std::expected<float, ErrorCode> gyro_y_res;
    std::expected<float, ErrorCode> gyro_z_res;

    inline constexpr bool hasFault()
    {
        return not(
            accel_x_res.has_value() && accel_y_res.has_value() && accel_z_res.has_value() && gyro_x_res.has_value() &&
            gyro_y_res.has_value() && gyro_z_res.has_value());
    }
};

void                            init();
void                            broadcast();
std::expected<float, ErrorCode> getAccelX();
std::expected<float, ErrorCode> getAccelY();
std::expected<float, ErrorCode> getAccelZ();
std::expected<float, ErrorCode> getGyroX();
std::expected<float, ErrorCode> getGyroY();
std::expected<float, ErrorCode> getGyroZ();
bool                            getAllInitsFailed();
// TODO: transformation matrix to reorient IMU to vehicle body frame (do for FSM and RSM as well)
} // namespace app::imus
