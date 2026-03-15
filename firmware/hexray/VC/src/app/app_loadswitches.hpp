#pragma once
#include "io_loadswitches.hpp"
#include <array>
#define NUM_EFUSE_CHANNELS 10

// Used to indicate the indeces of the array

namespace app::loadswitches
{
void efuse_broadcast();
} // namespace app::loadswitches
