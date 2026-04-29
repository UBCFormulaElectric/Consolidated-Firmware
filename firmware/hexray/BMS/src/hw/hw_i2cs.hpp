#pragma once

#include "hw_i2c.hpp"

#ifndef HAL_I2C_MODULE_ENABLED
struct I2C_HandleTypeDef
{
    int dummy;
};
#endif

namespace hw::i2c
{
static I2C_HandleTypeDef h_dummy_i2c = {};

bus i2c_bus_4(h_dummy_i2c);

device help_me(i2c_bus_4, 0, 0);

[[nodiscard]] bus &getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    return i2c_bus_4;
}
} // namespace hw::i2c