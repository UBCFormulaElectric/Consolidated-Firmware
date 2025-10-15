#pragma once
#include <stdbool.h>
#ifdef TARGET_EMBEDDED
#include "hw_i2c.h"

typedef struct
{
    const I2cDevice *imu_i2c_handle;
    double           x_accel_offset;
    double           y_accel_offset;
    double           z_accel_offset;
    double           yaw_offset;
    double           pitch_offset;
    double           roll_offset;
} imuConfig;
#else
#include "app_utils.h"
EMPTY_STRUCT(imuConfig)
#endif

/**
 * @brief turns on the accelerometer sensor on the imu to high perf mode
 * @return true if the write to turn on accelerometer is successful,
 * false otherwise
 */
ExitCode io_imu_init(void);

/**
 * @brief gets the linear acceleration in the x direction
 * @param x_acceleration value of x acceleration
 * @return if successful read
 */
ExitCode io_imu_getLinearAccelerationX(double *x_acceleration);

/**
 * @brief gets the linear acceleration in the y direction
 * @param y_acceleration value of y acceleration
 * @return if successful read
 */
ExitCode io_imu_getLinearAccelerationY(double *y_acceleration);

/**
 * @brief gets the linear acceleration in the y direction
 * @param z_acceleration value of z acceleration
 * @return if successful read
 */
ExitCode io_imu_getLinearAccelerationZ(double *z_acceleration);

/**
 * @brief Gets the angular velocity in the roll direction
 * @param roll_velocity Pointer to store the roll angular velocity
 * @return True if the read is successful, false otherwise
 */
ExitCode io_imu_getAngularVelocityRoll(double *roll_velocity);

/**
 * @brief Gets the angular velocity in the pitch direction
 * @param pitch_velocity Pointer to store the pitch angular velocity
 * @return True if the read is successful, false otherwise
 */
ExitCode io_imu_getAngularVelocityPitch(double *pitch_velocity);

/**
 * @brief Gets the angular velocity in the yaw direction
 * @param yaw_velocity Pointer to store the yaw angular velocity
 * @return True if the read is successful, false otherwise
 */
ExitCode io_imu_getAngularVelocityYaw(double *yaw_velocity);
