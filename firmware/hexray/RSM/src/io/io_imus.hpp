#pragma once
#include "util_errorCodes.hpp"
#include "io_imu.hpp"

extern io::imu::Imu imu_config;

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
    ExitCode accel_x_fault;
    ExitCode accel_y_fault;
    ExitCode accel_z_fault;
    ExitCode gyro_x_fault;
    ExitCode gyro_y_fault;
    ExitCode gyro_z_fault;
} RSMImuFaults;
