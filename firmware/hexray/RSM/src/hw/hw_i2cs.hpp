#pragma once
#include "hw_i2c.hpp"
#include <stm32h5xx_hal_i2c.h>

namespace hw::i2c
{
extern I2CBus    i2c_bus_2;
extern I2CDevice r_pump;
I2CBus          &getBusFromHandle(const I2C_HandleTypeDef *handle);
} // namespace hw::i2c