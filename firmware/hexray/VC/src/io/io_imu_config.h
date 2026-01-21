#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.h"
#endif
#include "io_imu.h"
#include "app_utils.h"

typedef struct
{
    double long_accel;
    double lat_accel;
    double z_accel;
    double yaw_rate;
    double pitch_rate;
    double roll_rate;
} ImuData;

typedef struct
{
    ExitCode long_accel_fault;
    ExitCode lat_accel_fault;
    ExitCode z_accel_fault;
    ExitCode roll_rate_fault;
    ExitCode pitch_rate_fault;
    ExitCode yaw_rate_fault;
} ImuFaults;

extern const imuConfig imu_config;
