#pragma once
#include <cstddef>
constexpr inline size_t NUM_EFUSE_CHANNELS = 10;

namespace app::loadswitches
{
enum EfuseChannel
{
    F_INV = 0,
    RSM,
    BMS,
    R_INV,
    DAM,
    FRONT,
    RL_PUMP,
    R_RAD,
    RR_PUMP,
    L_RAD
};
void efuse_broadcast();
} // namespace app::loadswitches
