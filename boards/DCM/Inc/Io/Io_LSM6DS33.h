#pragma once

/**
 * Driver for interfacing with the IMU LSM6DS33 via I2c
 *
 * The Application Note for this IMU can be found here:
 * https://www.pololu.com/file/0J1088/LSM6DS33-AN4682.pdf
 */

#include <stdint.h>
#include <stm32f3xx_hal.h>
#include <stdbool.h>
#include <App_SharedExitCode.h>

/**
 * Configures the LSM6DS33 IMU Gyro/Accelerometer chip
 * @param i2c_handle The handle for the I2C device the IMU is connected to
 * @return None
 */
HAL_StatusTypeDef Io_LSM6DS33_ConfigureImu(I2C_HandleTypeDef *i2c_handle);

/**
 * Read data from the IMU and update internal variables
 * @return ExitCode which is EXIT_CODE_OK if successful, EXIT_CODE_INVALID_ARGS if data not
 * read successfully
 */
ExitCode Io_LSM6DS33_ExtiCallback(void);

/**
 * Get x acceleration from imu
 * @return The acceleration (m/s^2) measured on the x-axis.
 */
float Io_LSM6DS33_GetAccelerationX(void);

/**
 * Get y acceleration from imu
 * @return The acceleration (m/s^2) measured on the y-axis.
 */
float Io_LSM6DS33_GetAccelerationY(void);

/**
 * Get z acceleration from imu
 * @return The acceleration (m/s^2) measured on the z-axis.
 */
float Io_LSM6DS33_GetAccelerationZ(void);
