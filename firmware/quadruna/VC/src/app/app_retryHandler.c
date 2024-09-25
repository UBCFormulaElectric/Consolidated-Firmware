#include <stdbool.h>

#include "app_retryHandler.h"
#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "app_canRx.h"
#include "app_canUtils.h"
#include "app_retryHandler.h"
#include "app_powerManager.h"

bool app_retryHandler_start(EfuseChannel efuse, const RetryConfig retry_configs[], RetryData retry_data[])
{
    bool go_to_init = false;

    switch (efuse)
    {
        case EFUSE_CHANNEL_SHDN:
            io_efuse_setChannel(EFUSE_CHANNEL_SHDN, false);

            retry_data[EFUSE_CHANNEL_SHDN].protocol_state = PROTOCOL_STATE_DEBOUNCE;
            break;
        case EFUSE_CHANNEL_LV:
            for (int efuse_channel = 0; efuse_channel < NUM_EFUSE_CHANNELS; efuse_channel++)
            {
                io_efuse_setChannel(efuse_channel, false);
                retry_data[efuse_channel].protocol_state = PROTOCOL_STATE_DEPENDENCY_WAITING;
            }

            go_to_init = true;

            retry_data[EFUSE_CHANNEL_LV].protocol_state = PROTOCOL_STATE_DEBOUNCE;
            break;
        case EFUSE_CHANNEL_PUMP:
            io_efuse_setChannel(EFUSE_CHANNEL_PUMP, false);
            retry_data[EFUSE_CHANNEL_PUMP].protocol_state = PROTOCOL_STATE_DEBOUNCE;
            break;
        case EFUSE_CHANNEL_INV_R:
        case EFUSE_CHANNEL_INV_L:
            io_efuse_setChannel(EFUSE_CHANNEL_INV_R, false);
            io_efuse_setChannel(EFUSE_CHANNEL_INV_L, false);
            io_efuse_setChannel(EFUSE_CHANNEL_SHDN, false);

            go_to_init = true;

            if (efuse == EFUSE_CHANNEL_INV_L)
            {
                retry_data[EFUSE_CHANNEL_INV_L].protocol_state = PROTOCOL_STATE_DEBOUNCE;
                retry_data[EFUSE_CHANNEL_INV_R].protocol_state = PROTOCOL_STATE_DEPENDENCY_WAITING;
            }
            else
            {
                retry_data[EFUSE_CHANNEL_INV_R].protocol_state = PROTOCOL_STATE_DEBOUNCE;
                retry_data[EFUSE_CHANNEL_INV_L].protocol_state = PROTOCOL_STATE_DEPENDENCY_WAITING;
            }
            retry_data[EFUSE_CHANNEL_SHDN].protocol_state = PROTOCOL_STATE_DEPENDENCY_WAITING;
            break;
        default:
            break;
    }
    return go_to_init;
}

void app_retryHandler_success(EfuseChannel efuse, const RetryConfig retry_configs[], RetryData retry_data[])
{
    switch (efuse)
    {
        case EFUSE_CHANNEL_SHDN:
            retry_data[EFUSE_CHANNEL_SHDN].protocol_state = PROTOCOL_STATE_OFF;
            break;
        case EFUSE_CHANNEL_LV:
            for (int efuse_channel = 0; efuse_channel < NUM_EFUSE_CHANNELS; efuse_channel++)
            {
                io_efuse_setChannel(efuse_channel, true);
                retry_data[efuse_channel].protocol_state = PROTOCOL_STATE_OFF;
            }
            break;
        case EFUSE_CHANNEL_PUMP:
            retry_data[EFUSE_CHANNEL_PUMP].protocol_state = PROTOCOL_STATE_OFF;
            break;
        case EFUSE_CHANNEL_INV_R:
        case EFUSE_CHANNEL_INV_L:
            io_efuse_setChannel(EFUSE_CHANNEL_INV_L, true);
            io_efuse_setChannel(EFUSE_CHANNEL_INV_R, true);
            io_efuse_setChannel(EFUSE_CHANNEL_SHDN, true);

            retry_data[EFUSE_CHANNEL_INV_L].protocol_state = PROTOCOL_STATE_OFF;
            retry_data[EFUSE_CHANNEL_INV_R].protocol_state = PROTOCOL_STATE_OFF;
            retry_data[EFUSE_CHANNEL_SHDN].protocol_state  = PROTOCOL_STATE_OFF;
            break;
        default:
            break;
    }
}