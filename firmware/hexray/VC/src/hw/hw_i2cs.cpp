#include "hw_i2cs.hpp"
#include "main.h"
#include <cassert>

constexpr hw::i2c::bus i2c_bus_4(hi2c4);
constexpr hw::i2c::bus i2c_bus_5(hi2c5);

const hw::i2c::device pwr_pump(i2c_bus_4, 0x10, 100);
const hw::i2c::device bat_mon(i2c_bus_5, 0x10 >> 1, 100);

const hw::i2c::bus &hw::i2c::getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    if (handle == &hi2c4)
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