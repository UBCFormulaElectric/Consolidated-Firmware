#include "app_efuse.h"

#include <stdbool.h>
#include <stddef.h>

#include "io_efuse.h"
#include "app_canTx.h"

static void (*const efuse_enabled_can_setters[NUM_EFUSE_CHANNELS])(bool) = {
    [EFUSE_CHANNEL_SHDN]   = app_canTx_VC_ShdnStatus_set,
    [EFUSE_CHANNEL_LV]     = app_canTx_VC_LvStatus_set,
    [EFUSE_CHANNEL_PUMP]   = app_canTx_VC_PumpStatus_set,
    [EFUSE_CHANNEL_AUX]    = app_canTx_VC_AuxStatus_set,
    [EFUSE_CHANNEL_INV_R]  = app_canTx_VC_InvRStatus_set,
    [EFUSE_CHANNEL_INV_L]  = app_canTx_VC_InvLStatus_set,
    [EFUSE_CHANNEL_TELEM]  = NULL,
    [EFUSE_CHANNEL_BUZZER] = NULL,
};

static void (*const efuse_current_can_setters[NUM_EFUSE_CHANNELS])(float) = {
    [EFUSE_CHANNEL_SHDN]   = app_canTx_VC_ShdnCurrent_set,
    [EFUSE_CHANNEL_LV]     = app_canTx_VC_LvCurrent_set,
    [EFUSE_CHANNEL_PUMP]   = app_canTx_VC_PumpCurrent_set,
    [EFUSE_CHANNEL_AUX]    = app_canTx_VC_AuxCurrent_set,
    [EFUSE_CHANNEL_INV_R]  = app_canTx_VC_InvRCurrent_set,
    [EFUSE_CHANNEL_INV_L]  = app_canTx_VC_InvLCurrent_set,
    [EFUSE_CHANNEL_TELEM]  = NULL,
    [EFUSE_CHANNEL_BUZZER] = NULL,
};

void app_efuse_broadcast(void)
{
    // run through each efuse, and broadcast the channel status and current
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse += 1)
    {
        bool  enabled = io_efuse_isChannelEnabled((EfuseChannel)efuse);
        float current = io_efuse_getChannelCurrent((EfuseChannel)efuse);

        if (efuse_enabled_can_setters[efuse] != NULL)
        {
            efuse_enabled_can_setters[efuse](enabled);
        }

        if (efuse_current_can_setters[efuse] != NULL)
        {
            efuse_current_can_setters[efuse](current);
        }
    }
}
