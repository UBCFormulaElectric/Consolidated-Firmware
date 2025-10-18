#include "hw_i2cs.h"

static I2cBus i2c_bus_1 = { .handle = &hi2c1 };

const I2cDevice rtc_i2c = { .bus = &i2c_bus_1, .target_address = 0x68, .timeout_ms = 10000 };

I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    if (handle == &hi2c1)
    {
        return &i2c_bus_1;
    }
    else
    {
        return NULL;
    }
}
