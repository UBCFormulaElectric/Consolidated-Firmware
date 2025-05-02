#include "hw_i2cs.h"
#include "main.h"

static I2cBus i2c_bus_1 = { .handle = &hi2c1 };
static I2cBus i2c_bus_2 = { .handle = &hi2c2 };

const I2cDevice imu_i2c           = { .bus            = &i2c_bus_2,
                                      .target_address = 0x6B,
                                      .timeout_ms     = 100 }; // target address is slave address specified in altium
const I2cDevice potentiometer_i2c = { .bus = &i2c_bus_1, .target_address = 0x2F, .timeout_ms = 100 };

I2cBus *hw_i2c_getBusFromHandle(
    const I2C_HandleTypeDef *handle) // this is defined for VC, need to define one for RSM and FSM too
{
    if (handle == &hi2c1)
    {
        return &i2c_bus_1;
    }
    else if (handle == &hi2c2)
    {
        return &i2c_bus_2;
    }
    else
    {
        return NULL;
    }
}
