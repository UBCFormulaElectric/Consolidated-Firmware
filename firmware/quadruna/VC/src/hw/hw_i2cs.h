#pragma once

#include "hw_i2c.h"

typedef enum
{
    HW_I2C_BUS_1,
    HW_I2C_BUS_2,
    HW_I2C_BUS_COUNT,
} I2cBus;

extern I2C_HandleTypeDef *const i2c_bus_handles[HW_I2C_BUS_COUNT];

extern const I2cDevice imu_i2c;
extern const I2cDevice potentiometer_i2c;
