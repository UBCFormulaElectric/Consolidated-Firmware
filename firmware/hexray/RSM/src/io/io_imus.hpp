#pragma once
#include "util_errorCodes.hpp"
#include "io_imu.hpp"

extern io::imu::Imu imu_config;

typedef struct
{
    std::expected<void, ErrorCode> accel_x_fault;
    std::expected<void, ErrorCode> accel_y_fault;
    std::expected<void, ErrorCode> accel_z_fault;
    std::expected<void, ErrorCode> gyro_x_fault;
    std::expected<void, ErrorCode> gyro_y_fault;
    std::expected<void, ErrorCode> gyro_z_fault;
} RSMImuFaults;
