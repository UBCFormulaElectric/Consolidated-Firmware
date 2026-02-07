
#pragma once

#include <hw_i2c.hpp>
#include "main.h"
#include <cassert>

namespace hw::i2c
{
extern I2CDevice bat_mon;
extern I2CDevice pwr_pump;
} // namespace hw::i2c
