#include "hw_i2cs.h"
#include <assert.h>

I2cBus    i2c_bus_1   = { .handle = &hi2c1 };
I2cDevice pwr_mon_i2c = { .bus = &i2c_bus_1, .target_address = 0x10, .timeout_ms = 30 };

I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    assert(handle == &hi2c1);
    return &i2c_bus_1;
}
