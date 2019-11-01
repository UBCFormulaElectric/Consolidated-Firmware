#ifndef IO_IMU_LSM6DS33
#define IO_IMU_LSM6DS33

/**
 * Driver for interfacing with the IMU LSM6DS33 via I2c
 *
 * The Application Note for this IMU can be found here:
 * https://www.pololu.com/file/0J1088/LSM6DS33-AN4682.pdf
 */

#include <stdint.h>
#include <stm32f3xx_hal.h>
#include <stdbool.h>

struct ImuData
{
    // Acceleration in m/s^2
    float accel_x;
    float accel_y;
    float accel_z;

    // The time (in ms) that this data was received
    uint32_t received_time_ms;
};

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
bool Io_Imu_LSM6DS33_readIMUData(void);

/**
 * Gets the data most recently read from the IMU
 *
 * @param       imu_data This should point to a struct that will be populated
 *                       with the most recently received IMU data
 * @return      true if we were able to retrieve valid data, false otherwise
 */
bool Io_Imu_LSM6DS33_getImuData(ImuData *imu_data);

#endif // IO_IMU_LSM6DS33
