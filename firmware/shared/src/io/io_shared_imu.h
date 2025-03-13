#pragma once

#include <stdbool.h>
#include "hw_i2c.h"


/**
 * @brief turns on the accelerometer sensor on the imu to high perf mode
 * @return true if the write to turn on accelerometer is successful,
 * false otherwise
 */
bool io_shared_imu_init(const I2cDevice *imu_i2c_handle);

/**
 * @brief gets the linear acceleration in the x direction
 * @param x_acceleration value of x acceleration
 * @return if successful read
 */
bool io_shared_imu_getLinearAccelerationX(const I2cDevice *imu_i2c_handle, float *x_acceleration);

/**
 * @brief gets the linear acceleration in the y direction
 * @param y_acceleration value of y acceleration
 * @return if successful read
 */
bool io_shared_imu_getLinearAccelerationY(const I2cDevice *imu_i2c_handle, float *y_acceleration);

/**
 * @brief gets the linear acceleration in the y direction
 * @param z_acceleration value of z acceleration
 * @return if successful read
 */
bool io_shared_imu_getLinearAccelerationZ(const I2cDevice *imu_i2c_handle, float *z_acceleration);

/**
 * @brief Gets the angular velocity in the roll direction
 * @param roll_velocity Pointer to store the roll angular velocity
 * @return True if the read is successful, false otherwise
 */
bool io_shared_imu_getAngularVelocityRoll(const I2cDevice *imu_i2c_handle, float *roll_velocity);

/**
 * @brief Gets the angular velocity in the pitch direction
 * @param pitch_velocity Pointer to store the pitch angular velocity
 * @return True if the read is successful, false otherwise
 */
bool io_shared_imu_getAngularVelocityPitch(const I2cDevice *imu_i2c_handle, float *pitch_velocity);

/**
 * @brief Gets the angular velocity in the yaw direction
 * @param yaw_velocity Pointer to store the yaw angular velocity
 * @return True if the read is successful, false otherwise
 */
bool io_shared_imu_getAngularVelocityYaw(const I2cDevice *imu_i2c_handle, float *yaw_velocity);