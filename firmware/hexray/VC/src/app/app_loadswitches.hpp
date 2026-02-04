#pragma once

namespace vc::app::loadswitches
{

enum class TPS28_EfuseChannel {
    F_INV = 0,
    R_INV,
    RSM,
    BMS,
    DAM,
    FRONT, // FSM and CRIT
    MISC,
    Count
};

enum class TPS25_EfuseChannel{ RR_PUMP, RL_PUMP, R_RAD, L_RAD, Count };

enum class EfuseChannel { TI_EfuseChannel = 0, ST_EfuseChannel };

static const void efuse_channel_setter(EfuseChannel *channel, bool status);
static const void efuse_current_setter(EfuseChannel &channel, float current);

} // namespace vc::app::loadswitches