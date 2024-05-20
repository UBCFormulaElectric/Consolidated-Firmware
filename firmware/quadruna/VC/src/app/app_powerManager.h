#include <stdbool.h>
#include "io_efuse.h"

typedef struct
{
    bool efuses[NUM_EFUSE_CHANNELS];
    bool pcm;
} PowerStateConfig;

void app_powerManager_updateConfig(PowerStateConfig power_manager_config);

void app_powerManager_updateEfuse(EfuseChannel channel, bool val);

void app_powerManager_updatePcm(bool val);

#ifdef TARGET_TEST
PowerStateConfig app_powerManager_getConfig(void);
bool             app_powerManager_getEfuse(EfuseChannel channel);
#endif