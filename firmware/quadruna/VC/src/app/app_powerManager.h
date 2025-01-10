#include <stdbool.h>
#include "io_efuse.h"

typedef struct {
    bool efuses[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

void app_powerManager_updateConfig(PowerStateConfig power_manager_config);

#ifdef TARGET_TEST
PowerStateConfig app_powerManager_getConfig(void);
bool             app_powerManager_getEfuse(EfuseChannel channel);
#endif