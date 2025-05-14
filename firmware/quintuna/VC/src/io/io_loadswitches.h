#pragma once

#include "io_loadswitch.h"

typedef enum
{
    // INV_RSM loadswitch
    EFUSE_CHANNEL_F_INV,
    EFUSE_CHANNEL_RSM,

    // INV_BMS loadswitch
    EFUSE_CHANNEL_BMS,
    EFUSE_CHANNEL_R_INV,

    // Front loadswitch
    EFUSE_CHANNEL_DAM,
    EFUSE_CHANNEL_FRONT,

    // TI loadswitches
    EFUSE_CHANNEL_RL_PUMP,
    EFUSE_CHANNEL_RR_PUMP,
    EFUSE_CHANNEL_F_PUMP,

    // Radiator Fan loadswitches
    EFUSE_CHANNEL_L_RAD,
    EFUSE_CHANNEL_R_RAD,

    NUM_EFUSE_CHANNELS
} LoadswitchChannel;

extern const ST_LoadSwitch inv_rsm_loadswitch;
extern const ST_LoadSwitch inv_bms_loadswitch;
extern const ST_LoadSwitch front_loadswitch;
extern const TI_LoadSwitch rl_pump_loadswitch;
extern const TI_LoadSwitch rr_pump_loadswitch;
extern const TI_LoadSwitch f_pump_loadswitch;
extern const ST_LoadSwitch rad_fan_loadswitch;

extern const Efuse efuse_channels[NUM_EFUSE_CHANNELS];