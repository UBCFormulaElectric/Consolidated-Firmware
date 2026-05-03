#include "hw_i2c.hpp"
#include "main.h"
#include <cassert>

namespace
{
I2C_HandleTypeDef      dummy_handle{};
constexpr hw::i2c::bus dummy_bus{ dummy_handle };
} // namespace

const hw::i2c::bus &hw::i2c::getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    (void)handle;
    assert(false);
    return dummy_bus;
}
