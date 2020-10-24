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
HAL_StatusTypeDef Io_Imu_LSM6DS33_configureImu(I2C_HandleTypeDef *i2c_handle);

/**
 * Read data from the IMU and update internal variables
 *
 * @return      true if we read data successfully, false otherwise
 */
bool Io_Imu_LSM6DS33_UpdateImuData(void);

/**
 * Get data received from imu in ImuData struct
 * @return ImuData struct *data
 */
struct ImuData *Io_Imu_LSM6DS33_GetImuData(void);

#endif // IO_IMU_LSM6DS33
