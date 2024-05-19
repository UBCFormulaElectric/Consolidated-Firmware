#include "app_efuse.h"
#include <stdbool.h>
#include "io_efuse.h"
#include "io_log.h"

static void (*const *efuse_enabled_can_setters)(bool);
static void (*const *efuse_current_can_setters)(float);

void app_efuse_init(
    void (*const enabled_setters[NUM_EFUSE_CHANNELS])(bool),
    void (*const current_setters[NUM_EFUSE_CHANNELS])(float))
{
    //    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse += 1)
    //    {
    //        efuse_enabled_can_setters[efuse] = enabled_setters[efuse];
    //        efuse_current_can_setters[efuse] = current_setters[efuse];
    //    }

    efuse_enabled_can_setters = enabled_setters;
    efuse_current_can_setters = current_setters;
}

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
