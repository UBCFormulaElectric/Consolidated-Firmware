#pragma once

#ifdef TARGET_EMBEDDED
#include "hw/i2cs.h"
#endif
#include "io/imu.h"
typedef struct
{
    float long_accel;
    float lat_accel;
    float z_accel;
    float yaw_rate;
    float pitch_rate;
    float roll_rate;
} ImuData;

extern const imuConfig imu_config;