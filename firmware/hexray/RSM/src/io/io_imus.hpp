#pragma once
#include "util_errorCodes.hpp"
#include "io_imu.hpp"

extern io::Imu imu_config;

typedef struct
{
    float x_lin_accel;
    float y_lin_accel;
    float z_lin_accel;
    float roll_ang_vel;
    float pitch_ang_vel;
    float yaw_ang_vel;
} RSMImuData;

typedef struct
{
    std::expected<void, ErrorCode> accel_x_fault;
    std::expected<void, ErrorCode> accel_y_fault;
    std::expected<void, ErrorCode> accel_z_fault;
    std::expected<void, ErrorCode> gyro_x_fault;
    std::expected<void, ErrorCode> gyro_y_fault;
    std::expected<void, ErrorCode> gyro_z_fault;
} RSMImuFaults;
