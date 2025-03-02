#include "hw_i2cs.h"
#include <assert.h>

I2cBus    bus_1    = { .handle = &hi2c1 };
I2cDevice device_1 = { .bus = &bus_1, .target_address = 0x6B, .timeout_ms = 40 };

I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    assert(handle == &hi2c1);
    return &bus_1;
}