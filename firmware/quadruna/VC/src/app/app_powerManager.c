#include "io_efuse.h"
#include "app_powerManager.h"
#include "app_canRx.h"
#include "io_pcm.h"

void app_powerManager_setState(PowerStateConfig state)
{
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_efuse_setChannel((EfuseChannel)efuse, state.efuses[efuse]);
    }
    io_pcm_set(state.pcm);
}
