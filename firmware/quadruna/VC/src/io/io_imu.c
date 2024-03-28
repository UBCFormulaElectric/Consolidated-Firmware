#include "io_imu.h"
#include "hw_i2c.h"
#include "app_canTx.h"
#include <math.h>
#include <stdio.h>

// Default sensitivity for LSM6DSM is 0.061 mg/digit
const float SENSITIVITY = 0.061f;

static I2cInterface *imu;

bool io_imu_init(I2cInterface * imu_in)
{
    imu = imu_in;
    return hw_i2c_is_target_ready(imu);
}

double io_imu_get_linear_acceleration_x()
{
    uint8_t x_data[2];

    // Read raw acceleration data
    hw_i2c_mem_read(imu, 0x28, 1, &x_data[0], 1); // LSB
    hw_i2c_mem_read(imu, 0x29, 1, &x_data[1], 1); // MSB
    int16_t x_raw = (int16_t)((int16_t)x_data[1] << 8 | x_data[0]);

    // Convert raw value to acceleration in m/s^2
    float x_acceleration = x_raw * SENSITIVITY * 9.81f / 1000.0f;

    app_canTx_VC_X_set(x_acceleration);
    return x_acceleration;
}

double io_imu_get_linear_acceleration_y()
{
    uint8_t y_data[2];

    // Read raw acceleration data
    hw_i2c_mem_read(imu, 0x2A, 1, &y_data[0], 1); // LSB
    hw_i2c_mem_read(imu, 0x2B, 1, &y_data[1], 1); // MSB
    int16_t y_raw = (int16_t)((int16_t)y_data[1] << 8 | y_data[0]);

    // Convert raw value to acceleration in m/s^2
    float y_acceleration = y_raw * SENSITIVITY * 9.81f / 1000.0f;

    app_canTx_VC_Y_set(y_acceleration);
    return y_acceleration;
}

double io_imu_get_linear_acceleration_z()
{
    uint8_t z_data[2];

    // Read raw acceleration data
    hw_i2c_mem_read(imu, 0x2C, 1, z_data, 1);
    hw_i2c_mem_read(imu, 0x2D, 1, z_data, 1);
    int16_t z_raw = (int16_t)(z_data[1] << 8 | z_data[0]);

    // Convert raw value to acceleration in m/s^2
    float z_acceleration = z_raw * SENSITIVITY * 9.81f / 1000.0f;

    app_canTx_VC_Z_set(z_acceleration);
    return z_acceleration;
}
