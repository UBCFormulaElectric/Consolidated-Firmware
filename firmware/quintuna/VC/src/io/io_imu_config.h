#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.h"
#endif
#include "io_imu.h"
typedef struct
{
    float long_accel;
    float lat_accel;
    float z_accel;
    float yaw_rate;
    float pitch_rate;
    float roll_rate;
} ImuData;

typedef struct
{
    ExitCode long_accel_fault;
    ExitCode lat_accel_fault;
    ExitCode z_accel_fault;
    ExitCode yaw_rate_fault;
    ExitCode pitch_rate_fault;
    ExitCode roll_rate_fault;
} ImuFaults;

extern const imuConfig imu_config;