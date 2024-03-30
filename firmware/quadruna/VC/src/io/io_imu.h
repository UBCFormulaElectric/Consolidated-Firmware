#pragma once

#include <stdbool.h>

/**
 * @brief turns on the accelerometer sensor on the imu to normal mode
 * @return true if the write to turn on accelerometer is successful,
 * false otherwise
 */
bool io_imu_init();

/**
 * @brief gets the linear acceleration in the x direction
 * @param x_acceleration value of x acceleration
 * @return if successful read
 */
bool io_imu_getLinearAccelerationX(float *x_acceleration);

/**
 * @brief gets the linear acceleration in the y direction
 * @param y_acceleration value of y acceleration
 * @return if successful read
 */
bool io_imu_getLinearAccelerationY(float *y_acceleration);

/**
 * @brief gets the linear acceleration in the y direction
 * @param z_acceleration value of z acceleration
 * @return if successful read
 */
bool io_imu_getLinearAccelerationZ(float *z_acceleration);
