#include <stdbool.h>

#include "app_retryHandler.h"
#include "io_efuse.h"
#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "app_canRx.h"
#include "app_canUtils.h"
#include "app_retryHandler.h"
#include "app_powerManager.h"

void apply_retry_protocol(RetryProtocol protocol, const RetryConfig retry_configs[], bool success)
{
    switch (protocol)
    {
        case RETRY_PROTOCOL_LV:
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, retry_configs[EFUSE_CHANNEL_LVPWR].state);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, success ? retry_configs[EFUSE_CHANNEL_DI_LHS].state : false);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, success ? retry_configs[EFUSE_CHANNEL_DI_RHS].state : false);
            io_efuse_setChannel(EFUSE_CHANNEL_AUX, success ? retry_configs[EFUSE_CHANNEL_AUX].state : false);
            break;
        case RETRY_PROTOCOL_INVERTERS:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, retry_configs[EFUSE_CHANNEL_DI_LHS].state);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, retry_configs[EFUSE_CHANNEL_DI_RHS].state);
            io_efuse_setChannel(EFUSE_CHANNEL_AUX, success ? retry_configs[EFUSE_CHANNEL_AUX].state : false);
            break;
        case RETRY_PROTOCOL_AIR:
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, retry_configs[EFUSE_CHANNEL_AIR].state);
            if (!success)
            {
                app_stateMachine_setNextState(app_initState_get());
            }
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, success ? retry_configs[EFUSE_CHANNEL_LVPWR].state : false);
            break;
        case RETRY_PROTOCOL_AUX:
            io_efuse_setChannel(EFUSE_CHANNEL_AUX, retry_configs[EFUSE_CHANNEL_AUX].state);
            if (!success)
            {
                app_stateMachine_setNextState(app_initState_get());
            }
            break;
        case RETRY_PROTOCOL_FANS:
            io_efuse_setChannel(EFUSE_CHANNEL_FAN ,retry_configs[EFUSE_CHANNEL_FAN].state);
            break;
        case RETRY_PROTOCOL_EMETER:
            break;
        case RETRY_PROTOCOL_DRS:
            break;
        default:
            break;
    }
}
