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

struct ImuData;

/* Function Definitions */
/**
 * Configures the LSM6DS33 IMU Gyro/Accelerometer chip
 *
 * @param      i2c_handle The handle for the I2C device the IMU is connected to
 * @return     None
 */
HAL_StatusTypeDef Io_LSM6DS33_ConfigureImu(I2C_HandleTypeDef *i2c_handle);

/**
 * Read data from the IMU and update internal variables
 *
 * @return      true if we read data successfully, false otherwise
 */
bool Io_LSM6DS33_UpdateImuData(void);

/**
 * Get x acceleration from imu
 * @return x acceleration in m/s^2
 */
float Io_LSM6DS33_GetAccelerationX(void);

/**
 * Get y acceleration from imu
 * @return y acceleration in m/s^2
 */
float Io_LSM6DS33_GetAccelerationY(void);

/**
 * Get z acceleration from imu
 * @return z acceleration in m/s^2
 */
float Io_LSM6DS33_GetAccelerationZ(void);
