#include "app_efuse.h"
#include <stdbool.h>
#include "io_efuse.h"

void app_efuse_broadcast(void)
{
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse += 1)
    {
        float current = io_efuse_getChannelCurrent(efuse);
        bool  enabled = io_efuse_isChannelEnabled(efuse);
    }
}