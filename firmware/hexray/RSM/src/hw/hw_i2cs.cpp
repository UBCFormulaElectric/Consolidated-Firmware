#include "hw_i2cs.hpp"
#include "hw_i2c.hpp"
#include "main.h"

//main.h not linking and hi2c2 from main.c not linking
//do I need init functions? I would need to use HAL functions though
namespace hw::i2c
{
    I2CBus i2c_bus_2(hi2c2);

    I2CDevice r_pump_i2c(i2c_bus_2, 0x2F, 100);

    I2CBus &getBusFromHandle(const I2C_HandleTypeDef *handle)
    {
        assert(handle == &hi2c2);
        return i2c_bus_2;
    }
}