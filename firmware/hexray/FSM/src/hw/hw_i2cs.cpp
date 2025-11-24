#include "hw_i2cs.hpp"
extern "C" {
    #include <assert.h>
}

I2C_HandleTypeDef hi2c1; // Should be in main.c????? just to make the code compile for now
I2cBus            i2c_bus_1 = { .handle = &hi2c1 };
I2cDevice         imu_i2c   = { .bus = &i2c_bus_1, .target_address = 0x6B, .timeout_ms = 40 };

I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    // assert(handle == &hi2c1);
    return &i2c_bus_1;
}