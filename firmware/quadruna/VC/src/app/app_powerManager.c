#include "io_efuse.h"
#include "app_powerManager.h"
#include "app_canRx.h"

typedef struct
{
    bool efuses[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

// TODO: efuse configs
static const PowerStateConfig power_states_config[NUM_POWER_STATES] = {
    [POWER_MANAGER_SHUTDOWN] = { .efuses = { [BLANK_EFUSE] = true } },
    [POWER_MANAGER_DRIVE]    = { .efuses = { [BLANK_EFUSE] = true } }
};

void app_powerManager_setState(PowerManagerState state)
{
    current_power_state = state;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_efuse_setChannel(efuse, power_states_config[state].efuses[efuse]);
    }
}

PowerManagerState app_powerManager_getState()
{
    return current_power_state;
}