#include "app_loadswitches.h"

#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#include "app_canTx.h"
#include "io_efuses.h"

static void (*const efuse_enabled_can_setters[NUM_EFUSE_CHANNELS])(bool) = {
    [EFUSE_CHANNEL_F_INV]   = app_canTx_VC_FrontInvertersStatus_set,
    [EFUSE_CHANNEL_RSM]     = app_canTx_VC_RSMStatus_set,
    [EFUSE_CHANNEL_BMS]     = app_canTx_VC_BMSStatus_set,
    [EFUSE_CHANNEL_R_INV]   = app_canTx_VC_RearInvertersStatus_set,
    [EFUSE_CHANNEL_DAM]     = app_canTx_VC_DAMStatus_set,
    [EFUSE_CHANNEL_FRONT]   = app_canTx_VC_FrontStatus_set,
    [EFUSE_CHANNEL_RL_PUMP] = app_canTx_VC_RearLeftPumpStatus_set,
    [EFUSE_CHANNEL_R_RAD]   = app_canTx_VC_RightRadiatorFanStatus_set
};

static void (*const efuse_current_can_setters[NUM_EFUSE_CHANNELS])(float) = {
    [EFUSE_CHANNEL_F_INV]   = app_canTx_VC_FrontInvertersCurrent_set,
    [EFUSE_CHANNEL_RSM]     = app_canTx_VC_RSMCurrent_set,
    [EFUSE_CHANNEL_BMS]     = app_canTx_VC_BMSCurrent_set,
    [EFUSE_CHANNEL_R_INV]   = app_canTx_VC_RearInvertersCurrent_set,
    [EFUSE_CHANNEL_DAM]     = app_canTx_VC_DAMCurrent_set,
    [EFUSE_CHANNEL_FRONT]   = app_canTx_VC_FrontCurrent_set,
    [EFUSE_CHANNEL_RL_PUMP] = app_canTx_VC_RearLeftPumpCurrent_set,
    [EFUSE_CHANNEL_R_RAD]   = app_canTx_VC_RightRadiatorFanCurrent_set
};

void app_efuse_broadcast(void)
{
    // run through each efuse, and broadcast the channel status and current
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse += 1)
    {
        const bool  enabled = io_efuse_isChannelEnabled(efuse_channels[efuse]);
        const float current = io_efuse_getChannelCurrent(efuse_channels[efuse]);

        assert(efuse_enabled_can_setters[efuse] != NULL);
        efuse_enabled_can_setters[efuse](enabled);
        assert(efuse_current_can_setters[efuse] != NULL);
        efuse_current_can_setters[efuse](current);
    }
}

// void app_loadswitch_enableFa
