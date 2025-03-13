#pragma once
#include "io_shared_imu.h"
#include <stdbool.h>

bool io_imu_init();

bool io_imu_getLinearAccelerationX(float *x_acceleration);

bool io_imu_getLinearAccelerationY(float *y_acceleration);

bool io_imu_getLinearAccelerationZ(float *z_acceleration);

bool io_imu_getAngularVelocityRoll(float *roll_velocity);

bool io_imu_getAngularVelocityPitch(float *pitch_velocity);

bool io_imu_getAngularVelocityYaw(float *yaw_velocity);