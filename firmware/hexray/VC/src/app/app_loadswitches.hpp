#pragma once

namespace vc::app::loadswitches
{

extern enum class TPS28_EfuseChannel : uint8_t {
    F_INV = 0x0U,
    R_INV,
    RSM,
    BMS,
    DAM,
    FRONT, // FSM and CRIT
    MISC,
    Count
};

extern enum class TPS25_EfuseChannel : uint8_t { RR_PUMP, RL_PUMP, R_RAD, L_RAD, Count };

extern enum class EfuseChannel : uint8_t { TI_EfuseChannel = 0, ST_EfuseChannel };

static const void efuse_channel_setter(EfuseChannel *channel, bool status);
static const void efuse_current_setter(EfuseChannel &channel, float current);

} // namespace vc::app::loadswitches