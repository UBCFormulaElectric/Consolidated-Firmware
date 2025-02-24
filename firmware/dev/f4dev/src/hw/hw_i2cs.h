#pragma once

#include <stdbool.h>
#include "main.h"

// TODO: Setup I2Cs.
typedef enum
{
    HW_I2C_3,
    HW_I2C_BUS_COUNT,
    HW_I2C_UNSPECIFIED
} I2cBus;

extern I2C_HandleTypeDef *i2c_bus_handles[HW_I2C_BUS_COUNT];
