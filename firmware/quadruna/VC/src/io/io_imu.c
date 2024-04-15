#include "io_imu.h"
#include "hw_i2c.h"
#include <math.h>
#include <stdio.h>

// Default sensitivity for LSM6DSM is 0.061 mg/digit
const float SENSITIVITY = 0.061f;

extern I2C_HandleTypeDef hi2c2;
static I2cInterface      imu = { .i2c_handle = &hi2c2, .target_address = 0x6B, .timeout_ms = 100 };

bool io_imu_init()
{
    if (hw_i2c_isTargetReady(&imu))
    {
        uint8_t buffer = 0x40; // turn on accelerometer to normal mode
        return hw_i2c_memWrite(&imu, 0x10, &buffer, 1);
    }
    return false;
}

bool io_imu_getLinearAccelerationX(float *x_acceleration)
{
    uint8_t x_data[2];
    bool    is_read_successful = hw_i2c_memRead(&imu, 0x28, x_data, 2);

    if (!is_read_successful)
    {
        return false;
    }

    // Convert raw value to acceleration in m/s^2
    int16_t x_raw   = (int16_t)(x_data[1] << 8 | x_data[0]);
    *x_acceleration = x_raw * SENSITIVITY * 9.81f / 1000.0f;
    return true;
}

bool io_imu_getLinearAccelerationY(float *y_acceleration)
{
    uint8_t y_data[2];
    bool    is_read_successful = hw_i2c_memRead(&imu, 0x2A, y_data, 2);

    if (!is_read_successful)
    {
        return false;
    }

    // Convert raw value to acceleration in m/s^2
    int16_t y_raw   = (int16_t)(y_data[1] << 8 | y_data[0]);
    *y_acceleration = y_raw * SENSITIVITY * 9.81f / 1000.0f;
    return true;
}

bool io_imu_getLinearAccelerationZ(float *z_acceleration)
{
    uint8_t z_data[2];
    bool    is_read_successful = hw_i2c_memRead(&imu, 0x2C, z_data, 2);

    if (!is_read_successful)
    {
        return false;
    }

    // Convert raw value to acceleration in m/s^2 and subtract force of gravity
    int16_t z_raw   = (int16_t)(z_data[1] << 8 | z_data[0]);
    *z_acceleration = z_raw * SENSITIVITY * 9.81f / 1000.0f - 9.81f;
    return true;
}
