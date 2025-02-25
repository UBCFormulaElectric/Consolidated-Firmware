#pragma once

#include <stdbool.h>
#include "main.h"

typedef enum
{
    HW_I2C_BUS_1,
    HW_I2C_BUS_2,
    HW_I2C_BUS_COUNT
} I2cBus;

extern I2C_HandleTypeDef *i2c_bus_handles[HW_I2C_BUS_COUNT];
