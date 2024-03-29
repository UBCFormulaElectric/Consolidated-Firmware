#pragma once

#include <stdbool.h>

/**
 * @brief initalizes the i2c interface
 * @param i2c the i2c handle
 */
bool io_imu_init();

/**
 * @brief turns on the accelerometer sensor on the imu to normal mode
 */
bool io_imu_init_accelerometer();

/**
 * @brief gets the linear acceleration in the x direction
 */
double io_imu_get_linear_acceleration_x();

/**
 * @brief gets the linear acceleration in the y direction
 */
double io_imu_get_linear_acceleration_y();

/**
 * @brief gets the linear acceleration in the y direction
 */
double io_imu_get_linear_acceleration_z();
