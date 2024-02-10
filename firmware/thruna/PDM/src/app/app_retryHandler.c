#include <stdbool.h>

#include "app_retryHandler.h"
#include "io_efuse.h"
#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "app_powerManager.h"

void apply_retry_protocol(RetryProtocol protocol, bool isOn)
{
    switch (protocol)
    {
        case RETRY_PROTOCOL_LV:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, isOn);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, isOn);
            io_efuse_setChannel(EFUSE_CHANNEL_AUX, isOn);
            break;
        case RETRY_PROTOCOL_INVERTERS:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, isOn);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, isOn);
            io_efuse_setChannel(EFUSE_CHANNEL_AUX, isOn);
            break;
        case RETRY_PROTOCOL_AIR:
            if (!isOn) {
                app_stateMachine_init(app_initState_get());
            }
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, isOn);
            break;
        case RETRY_PROTOCOL_AUX:
            if (!isOn) {
                app_stateMachine_init(app_initState_get());
            }
            break;
        case RETRY_PROTOCOL_FANS:
            break;
        case RETRY_PROTOCOL_EMETER:
            break;
        case RETRY_PROTOCOL_DRS:
            break;
        default:
            break;
    }
}
