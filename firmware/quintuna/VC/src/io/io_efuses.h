#pragma once

#include "io_efuse/io_efuse.h"

typedef enum
{
    // INV_RSM loadswitch
    EFUSE_CHANNEL_F_INV = 0u,
    EFUSE_CHANNEL_RSM,

    // INV_BMS loadswitch
    EFUSE_CHANNEL_BMS,
    EFUSE_CHANNEL_R_INV,

    // Front loadswitch
    EFUSE_CHANNEL_DAM,
    EFUSE_CHANNEL_FRONT,

    // TI loadswitches
    EFUSE_CHANNEL_RL_PUMP,

    // Radiator Fan loadswitches
    EFUSE_CHANNEL_R_RAD,

    NUM_EFUSE_CHANNELS
} LoadswitchChannel;

extern Efuse *const efuse_channels[NUM_EFUSE_CHANNELS];
