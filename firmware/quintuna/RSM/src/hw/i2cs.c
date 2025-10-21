#include "main.h"
#include "hw/i2cs.h"
#include <assert.h>

static I2cBus i2c_bus_1 = { .handle = &hi2c1 };
static I2cBus i2c_bus_3 = { .handle = &hi2c3 };

const I2cDevice imu_i2c    = { .bus = &i2c_bus_1, .target_address = 0x6B, .timeout_ms = 100 };
const I2cDevice r_pump_i2c = { .bus = &i2c_bus_3, .target_address = 0x2F, .timeout_ms = 100 };

I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    if (handle == &hi2c1)
    {
        return &i2c_bus_1;
    }
    if (handle == &hi2c3)
    {
        return &i2c_bus_3;
    }
    assert(false);
    return NULL;
}
