#pragma once
#include "hw_i2c.h"

typedef enum
{
    HW_I2C_BUS_1,
    HW_I2C_BUS_COUNT
} I2cBus1;

extern I2C_HandleTypeDef *i2c_bus_handles[HW_I2C_BUS_COUNT];
