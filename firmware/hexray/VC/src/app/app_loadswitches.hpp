#pragma once
#include "io_loadswitches.hpp"
#include <array>

using namespace io;
#define NUM_EFUSE_CHANNELS 10
std::array<io::Efuse *const, [NUM_EFUSE_CHANNELS]> efuse_channels = {
    &F_INV_Efuse,
    &RSM_Efuse,
    &BMS_Efuse,
    &R_INV_Efuse,
    &DAM_Efuse,
    &FRONT_Efuse,
    &RL_PUMP_Efuse,
    &R_RAD_Efuse,
    &RR_PUMP_Efuse,
    &L_RAD_Efuse,
};

namespace app::loadswitches
{
void efuse_broadcast();
} // namespace app::loadswitches