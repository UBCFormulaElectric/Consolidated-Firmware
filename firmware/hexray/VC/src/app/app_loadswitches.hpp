#pragma once
#include "io_loadswitches.hpp"
using namespace io;
#define NUM_EFUSE_CHANNELS 10

namespace vc::app::loadswitches
{
void efuse_broadcast();
} // namespace vc::app::loadswitches