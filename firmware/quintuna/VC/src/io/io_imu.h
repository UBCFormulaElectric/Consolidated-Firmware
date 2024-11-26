#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
extern I2C_HandleTypeDef hi2c2;
#endif

class Imu
{
    /**
     * @brief turns on the accelerometer sensor on the imu to normal mode
     * @return true if the write to turn on accelerometer is successful,
     * false otherwise
     */
    bool init();

    /**
     * @brief gets the linear acceleration in the x direction
     * @param x_acceleration value of x acceleration
     * @return if successful read
     */
    bool getLinearAccelerationX(float *x_acceleration);

    /**
     * @brief gets the linear acceleration in the y direction
     * @param y_acceleration value of y acceleration
     * @return if successful read
     */
    bool getLinearAccelerationY(float *y_acceleration);

    /**
     * @brief gets the linear acceleration in the y direction
     * @param z_acceleration value of z acceleration
     * @return if successful read
     */
    bool getLinearAccelerationZ(float *z_acceleration);
};
