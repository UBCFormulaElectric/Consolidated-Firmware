#include "hw_i2cs.hpp"
#include "main.h"
#include <stm32h5xx_hal_spi.h>

namespace hw::i2c
{
I2CBus i2c_bus_2(hi2c2);

I2CDevice r_pump(i2c_bus_2, 0x2F, 100);

I2CBus &getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    assert(handle == &hi2c2);
    return i2c_bus_2;
}

} // namespace hw::i2c