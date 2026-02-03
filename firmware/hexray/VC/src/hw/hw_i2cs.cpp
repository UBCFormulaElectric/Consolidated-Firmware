#include "hw_i2cs.hpp"
#include "main.h"

namespace hw::i2c
{
I2CBus i2c_bus_4(hi2c4);
I2CBus i2c_bus_5(hi2c5);

I2CDevice bat_mon(i2c_bus_4, 0x10, 100);
I2CDevice pwr_pump(i2c_bus_5, 0x10, 100);

I2CBus &getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    if(handle == &hi2c4) 
    {
        return i2c_bus_4;
    }
    if (handle == &hi2c5) 
    {
        return i2c_bus_5;
    }
    // fallback
    assert(false);
    return i2c_bus_4;
}
} // namespace hw::i2c
