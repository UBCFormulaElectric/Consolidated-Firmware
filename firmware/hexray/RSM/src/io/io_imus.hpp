#pragma once
#include "util_errorCodes.hpp"
#include "io_imu.hpp"

extern const io::imu::Imu imu_config;

typedef struct
{
    std::expected<float, ErrorCode> accel_x_res;
    std::expected<float, ErrorCode> accel_y_res;
    std::expected<float, ErrorCode> accel_z_res;
    std::expected<float, ErrorCode> gyro_x_res;
    std::expected<float, ErrorCode> gyro_y_res;
    std::expected<float, ErrorCode> gyro_z_res;
} RSMImuFaults;
