#include "io_imu.h"
#include "hw_i2c.h"
#include "app_canTx.h"
#include <math.h>
#include <stdio.h>

extern I2C_HandleTypeDef hi2c2;
static I2cInterface     *imu;

// Default sensitivity for LSM6DSM is 0.061 mg/digit
const float SENSITIVITY = 0.061f;

bool io_imu_init()
{
    imu->i2c_handle     = &hi2c2;
    imu->target_address = 0x6B;
    imu->timeout_ms     = 100;

    return hw_i2c_is_target_ready(imu);
}

bool io_imu_init_accelerometer()
{
    uint8_t buffer[1];
    buffer[0] = 0x40; // turn on accelerometer to normal mode

    return hw_i2c_mem_write(imu, 0x10, 1, buffer, sizeof(buffer));
}

void read_register(uint8_t *rx_buffer, uint16_t mem_addr)
{
    hw_i2c_mem_read(imu, mem_addr, 1, &rx_buffer[0], 1);         // LSB
    hw_i2c_mem_read(imu, (mem_addr + 0x1), 1, &rx_buffer[1], 1); // MSB
}

double io_imu_acceleration_x()
{
    uint8_t x_data[2];
    read_register(x_data, 0x28);
    int16_t x_raw = (int16_t)((int16_t)x_data[1] << 8 | x_data[0]);

    // Convert raw value to acceleration in m/s^2
    float x_acceleration = x_raw * SENSITIVITY * 9.81f / 1000.0f;

    app_canTx_VC_X_set(x_acceleration);
    return x_acceleration;
}

double io_imu_acceleration_y()
{
    uint8_t y_data[2];
    read_register(y_data, 0x2A);
    int16_t y_raw = (int16_t)((int16_t)y_data[1] << 8 | y_data[0]);

    // Convert raw value to acceleration in m/s^2
    float y_acceleration = y_raw * SENSITIVITY * 9.81f / 1000.0f;

    app_canTx_VC_Y_set(y_acceleration);
    return y_acceleration;
}

double io_imu_acceleration_z()
{
    uint8_t z_data[2];
    read_register(z_data, 0x2C);
    int16_t z_raw = (int16_t)(z_data[1] << 8 | z_data[0]);

    // Convert raw value to acceleration in m/s^2
    float z_acceleration = z_raw * SENSITIVITY * 9.81f / 1000.0f;

    app_canTx_VC_Z_set(z_acceleration);
    return z_acceleration;
}
