#include <stdbool.h>

#include "app_retryHandler.h"
#include "io_efuse.h"
#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "app_canRx.h"
#include "app_canUtils.h"
#include "app_retryHandler.h"
#include "app_powerManager.h"

bool app_retryHandler_start(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[])
{
    bool go_to_init = false;

    switch (protocol)
    {
        case RETRY_PROTOCOL_SHDN:
            io_efuse_setChannel(EFUSE_CHANNEL_SHDN, false);

            retry_data[EFUSE_CHANNEL_SHDN].protocol_state = PROTOCOL_STATE_DEBOUNCE;
            break;
        case RETRY_PROTOCOL_LV:
            for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
            {
                io_efuse_setChannel(efuse, false);
                retry_data[efuse].protocol_state = PROTOCOL_STATE_DEPENDENCY_WAITING;
            }

            go_to_init = true;

            retry_data[EFUSE_CHANNEL_LV].protocol_state = PROTOCOL_STATE_DEBOUNCE;
            break;
        case RETRY_PROTOCOL_PUMP:
            io_efuse_setChannel(EFUSE_CHANNEL_PUMP, false);
            retry_data[EFUSE_CHANNEL_PUMP].protocol_state = PROTOCOL_STATE_DEBOUNCE;
            break;
        case RETRY_PROTOCOL_INV_R:
            io_efuse_setChannel(EFUSE_CHANNEL_INV_R, false);
            io_efuse_setChannel(EFUSE_CHANNEL_INV_L, false);
            io_efuse_setChannel(EFUSE_CHANNEL_SHDN, false);

            go_to_init = true;

            retry_data[EFUSE_CHANNEL_INV_R].protocol_state = PROTOCOL_STATE_DEBOUNCE;
            retry_data[EFUSE_CHANNEL_INV_L].protocol_state = PROTOCOL_STATE_DEPENDENCY_WAITING;
            retry_data[EFUSE_CHANNEL_SHDN].protocol_state  = PROTOCOL_STATE_DEPENDENCY_WAITING;
            break;
        case RETRY_PROTOCOL_INV_L:
            io_efuse_setChannel(EFUSE_CHANNEL_INV_R, false);
            io_efuse_setChannel(EFUSE_CHANNEL_INV_L, false);
            io_efuse_setChannel(EFUSE_CHANNEL_SHDN, false);

            go_to_init = true;

            retry_data[EFUSE_CHANNEL_INV_L].protocol_state = PROTOCOL_STATE_DEBOUNCE;
            retry_data[EFUSE_CHANNEL_INV_R].protocol_state = PROTOCOL_STATE_DEPENDENCY_WAITING;
            retry_data[EFUSE_CHANNEL_SHDN].protocol_state  = PROTOCOL_STATE_DEPENDENCY_WAITING;
            break;
        default:
            break;
    }
    return go_to_init;
}

void app_retryHandler_success(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[])
{
    switch (protocol)
    {
        case RETRY_PROTOCOL_SHDN:
            retry_data[EFUSE_CHANNEL_SHDN].protocol_state = PROTOCOL_STATE_OFF;
            break;
        case RETRY_PROTOCOL_LV:
            for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
            {
                io_efuse_setChannel(efuse, true);
                retry_data[efuse].protocol_state = PROTOCOL_STATE_OFF;
            }
            break;
        case RETRY_PROTOCOL_PUMP:
            retry_data[EFUSE_CHANNEL_PUMP].protocol_state = PROTOCOL_STATE_OFF;
            break;
        case RETRY_PROTOCOL_INV_R:
        case RETRY_PROTOCOL_INV_L:
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