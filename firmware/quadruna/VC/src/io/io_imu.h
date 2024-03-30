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
 */
float io_imu_getLinearAccelerationX();

/**
 * @brief gets the linear acceleration in the y direction
 */
float io_imu_getLinearAccelerationY();

/**
 * @brief gets the linear acceleration in the y direction
 */
float io_imu_getLinearAccelerationZ();
