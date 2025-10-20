#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.h"
#endif
#include "io_imu.h"
typedef struct
{
    double long_accel;
    double lat_accel;
    double z_accel;
    double yaw_rate;
    double pitch_rate;
    double roll_rate;
} ImuData;

extern const imuConfig imu_config;