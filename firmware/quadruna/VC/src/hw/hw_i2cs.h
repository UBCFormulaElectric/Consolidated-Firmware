#pragma once

typedef enum
{
    HW_I2C_BUS_1,
    HW_I2C_BUS_2,
    HW_I2C_BUS_COUNT,
} I2cBus;

typedef enum
{
    HW_I2C_DEVICE_IMU,
    HW_I2C_DEVICE_POTENTIOMETER,
    HW_I2C_DEVICE_COUNT
} I2cDevice;
