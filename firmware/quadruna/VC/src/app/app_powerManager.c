#include "io_efuse.h"
#include "app_powerManager.h"
#include "io_pcm.h"
#include "app_canTx.h"

static PowerStateConfig state;

void app_powerManager_updateState(const PowerStateConfig new_state)
{
    state = new_state;
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_efuse_setChannel((EfuseChannel)efuse, state.efuses[efuse]);
    }
    io_pcm_set(state.pcm);

    app_canTx_VC_BuzzerOn_set(new_state.efuses[EFUSE_CHANNEL_BUZZER]);
}

void app_powerManager_updateEfuse(EfuseChannel channel, bool val)
{
    state.efuses[channel] = val;
    io_efuse_setChannel(channel, val);

    app_canTx_VC_BuzzerOn_set(state.efuses[EFUSE_CHANNEL_BUZZER]);
}

void app_powerManager_updatePcm(bool val)
{
    state.pcm = val;
    io_pcm_set(val);
}
