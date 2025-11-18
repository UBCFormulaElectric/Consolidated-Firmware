#include "hw_i2cs.h"
#include "main.h"
#include <assert.h>

static I2cBus   i2c_bus_1 = { .handle = &hi2c2 };
static I2cBus   i2c_bus_2 = { .handle = &hi2c3 };
static I2cBus   i2c_bus_3 = { .handle = &hi2c4 };
static I2cBus   i2c_bus_4 = { .handle = &hi2c5 };
const I2cDevice pwr_mtr   = { .bus = &i2c_bus_4, .target_address = 0x01, .timeout_ms = 100 };

const I2cDevice imu     = { .bus = &i2c_bus_2, .target_address = 0x6B, .timeout_ms = 100 };
const I2cDevice bat_mtr = { .bus            = &i2c_bus_1,
                            .target_address = (0x10 >> 1),
                            .timeout_ms     = 100 }; // Data sheet for TI is a bitch
const I2cDevice pumps   = { .bus = &i2c_bus_3, .target_address = 0x2D, .timeout_ms = 100 };

I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    if (handle == &hi2c2)
    {
        return &i2c_bus_1;
    }
    else if (handle == &hi2c3)
    {
        return &i2c_bus_2;
    }
    else if (handle == &hi2c4)
    {
        return &i2c_bus_3;
    }
    else if (handle == &hi2c5)
    {
        return &i2c_bus_4;
    }
    else
    {
        assert(false);
        return NULL;
    }
}
