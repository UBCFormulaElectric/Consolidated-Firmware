#pragma once
#include "util_errorCodes.hpp"

extern io::imu::Imu imu_config();

typedef struct
{
    ExitCode accel_x_fault;
    ExitCode accel_y_fault;
    ExitCode accel_z_fault;
    ExitCode gyro_x_fault;
    ExitCode gyro_y_fault;
    ExitCode gyro_z_fault;
} RSMImuFaults;