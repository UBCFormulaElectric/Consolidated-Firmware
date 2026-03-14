#pragma once
#include "io_loadswitches.hpp"
#include <array>
#define NUM_EFUSE_CHANNELS 10

// Used to indicate the indeces of the array
enum EfuseChannel
{
    F_INV_Efuse = 0,
    RSM_Efuse,
    BMS_Efuse,
    R_INV_Efuse,
    DAM_Efuse,
    FRONT_Efuse,
    RL_PUMP_Efuse,
    R_RAD_Efuse,
    RR_PUMP_Efuse,
    L_RAD_Efuse
};

namespace app::loadswitches
{
void efuse_broadcast();
} // namespace app::loadswitches
