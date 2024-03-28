#pragma once

#include <stdbool.h>
#include "hw_i2c.h"

bool io_imu_init();

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
