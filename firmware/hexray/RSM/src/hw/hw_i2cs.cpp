#include "hw_i2cs.hpp"
#include "main.h"
#include <cassert>

constexpr hw::i2c::bus    i2c_bus_2(hi2c2);
constexpr hw::i2c::device r_pump(i2c_bus_2, 0x2F, 100);

const hw::i2c::bus &getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    assert(handle == &hi2c2);
    return i2c_bus_2;
}