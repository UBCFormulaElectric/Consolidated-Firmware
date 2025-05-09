#pragma once
#include "io_imu.h"

extern const imuConfig imu_config;
bool                   io_imu_getLinAccel(float *x, float *y, float *z);
bool                   io_imu_getAngVel(float *x, float *y, float *z);