#pragma once

#include "io_imu.hpp" 
#include "util_errorCodes.hpp"

extern const imuConfig imu_config; 

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
    ExitCode x_lin_accel_fault;
    ExitCode y_lin_accel_fault;
    ExitCode z_lin_accel_fault;
    ExitCode roll_ang_vel_fault;
    ExitCode pitch_ang_vel_fault;
    ExitCode yaw_ang_vel_fault; 
}RSMImuFaults;
