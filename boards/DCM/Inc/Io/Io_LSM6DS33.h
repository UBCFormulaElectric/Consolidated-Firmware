#pragma once

/**
 * Driver for interfacing with the Imu LSM6DS33 via I2c
 *
 * The Application Note for this Imu can be found here:
 * https://www.pololu.com/file/0J1088/LSM6DS33-AN4682.pdf
 */

#include <stdbool.h>
#include <stdint.h>
#include <stm32f4xx_hal.h>

#include "App_SharedExitCode.h"

/**
 * Get x acceleration from Imu
 * @return The acceleration (m/s^2) measured on the x-axis.
 */
float Io_LSM6DS33_GetAccelerationX(void);

/**
 * Get y acceleration from Imu
 * @return The acceleration (m/s^2) measured on the y-axis.
 */
float Io_LSM6DS33_GetAccelerationY(void);

/**
 * Get z acceleration from Imu
 * @return The acceleration (m/s^2) measured on the z-axis.
 */
float Io_LSM6DS33_GetAccelerationZ(void);
