#pragma once

#include <stdbool.h>

/**
 * @brief initalizes the i2c interface
 */
bool io_imu_init();

/**
 * @brief turns on the accelerometer sensor on the imu to normal mode
 */
bool io_imu_init_accelerometer();

/**
 * @brief gets the linear acceleration in the x direction
 */
void io_imu_transmit_linear_acceleration();
