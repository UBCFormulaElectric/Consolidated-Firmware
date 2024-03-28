#include <stdbool.h>

#include "app_retryHandler.h"
#include "io_efuse.h"
#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "app_canRx.h"
#include "app_canUtils.h"
#include "app_retryHandler.h"
#include "app_powerManager.h"

bool app_retry_handler_start(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[])
{
    bool go_to_init = false;
    switch (protocol)
    {
        // turn off/on lv and inverters
        case RETRY_PROTOCOL_LV:
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, false);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, false);

            io_efuse_standbyReset(EFUSE_CHANNEL_LVPWR);
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, true);

            go_to_init = true;

            retry_data[EFUSE_CHANNEL_LVPWR].protocol_state  = PROTOCOL_STATE_CALC_AVG;
            retry_data[EFUSE_CHANNEL_DI_LHS].protocol_state = PROTOCOL_STATE_WAITING;
            retry_data[EFUSE_CHANNEL_DI_RHS].protocol_state = PROTOCOL_STATE_WAITING;
            break;
        // turn off inverters and aux
        case RETRY_PROTOCOL_DI_LHS:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, false);

            io_efuse_standbyReset(EFUSE_CHANNEL_DI_LHS);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, true);

            retry_data[EFUSE_CHANNEL_DI_LHS].protocol_state = PROTOCOL_STATE_CALC_AVG;
            retry_data[EFUSE_CHANNEL_DI_RHS].protocol_state = PROTOCOL_STATE_WAITING;
            retry_data[EFUSE_CHANNEL_AIR].protocol_state    = PROTOCOL_STATE_WAITING;
            break;
        // turn off inverters and aux
        case RETRY_PROTOCOL_DI_RHS:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, false);

            io_efuse_standbyReset(EFUSE_CHANNEL_DI_RHS);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, true);

            retry_data[EFUSE_CHANNEL_DI_RHS].protocol_state = PROTOCOL_STATE_CALC_AVG;
            retry_data[EFUSE_CHANNEL_DI_LHS].protocol_state = PROTOCOL_STATE_WAITING;
            retry_data[EFUSE_CHANNEL_AIR].protocol_state    = PROTOCOL_STATE_WAITING;
            break;
        // turn off air and lv, go to init state
        case RETRY_PROTOCOL_AIR:
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, false);
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, false);

            io_efuse_standbyReset(EFUSE_CHANNEL_AIR);
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, true);

            go_to_init = true;

            retry_data[EFUSE_CHANNEL_AIR].protocol_state   = PROTOCOL_STATE_CALC_AVG;
            retry_data[EFUSE_CHANNEL_LVPWR].protocol_state = PROTOCOL_STATE_WAITING;
            break;
        // go back to init state
        case RETRY_PROTOCOL_AUX:
            io_efuse_setChannel(EFUSE_CHANNEL_AUX, false);

            go_to_init = true;

            io_efuse_standbyReset(EFUSE_CHANNEL_AUX);
            io_efuse_setChannel(EFUSE_CHANNEL_AUX, true);

            retry_data[EFUSE_CHANNEL_AUX].protocol_state = PROTOCOL_STATE_CALC_AVG;
            break;
        // turn off fans
        case RETRY_PROTOCOL_FANS:
            io_efuse_setChannel(EFUSE_CHANNEL_FAN, false);

            io_efuse_standbyReset(EFUSE_CHANNEL_FAN);
            io_efuse_setChannel(EFUSE_CHANNEL_FAN, true);

            retry_data[EFUSE_CHANNEL_FAN].protocol_state = PROTOCOL_STATE_CALC_AVG;
            break;
        // turn off emeter
        case RETRY_PROTOCOL_EMETER:
            io_efuse_setChannel(EFUSE_CHANNEL_EMETER, false);

            io_efuse_standbyReset(EFUSE_CHANNEL_EMETER);
            io_efuse_setChannel(EFUSE_CHANNEL_EMETER, true);

            retry_data[EFUSE_CHANNEL_EMETER].protocol_state = PROTOCOL_STATE_CALC_AVG;
            break;
        // turn off drs
        case RETRY_PROTOCOL_DRS:
            io_efuse_setChannel(EFUSE_CHANNEL_DRS, false);

            io_efuse_standbyReset(EFUSE_CHANNEL_DRS);
            io_efuse_setChannel(EFUSE_CHANNEL_DRS, true);

            retry_data[EFUSE_CHANNEL_DRS].protocol_state = PROTOCOL_STATE_CALC_AVG;
        default:
            break;
    }
    return go_to_init;
}

void app_retry_handler_success(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[])
{
    switch (protocol)
    {
        case RETRY_PROTOCOL_LV:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, true);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, true);

            retry_data[EFUSE_CHANNEL_LVPWR].protocol_state  = PROTOCOL_STATE_OFF;
            retry_data[EFUSE_CHANNEL_DI_LHS].protocol_state = PROTOCOL_STATE_OFF;
            retry_data[EFUSE_CHANNEL_DI_RHS].protocol_state = PROTOCOL_STATE_OFF;
            break;
        case RETRY_PROTOCOL_DI_LHS:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, true);
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, true);

            retry_data[EFUSE_CHANNEL_DI_LHS].protocol_state = PROTOCOL_STATE_OFF;
            retry_data[EFUSE_CHANNEL_DI_RHS].protocol_state = PROTOCOL_STATE_OFF;
            retry_data[EFUSE_CHANNEL_AIR].protocol_state    = PROTOCOL_STATE_OFF;
            break;
        case RETRY_PROTOCOL_DI_RHS:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, true);
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, true);

            retry_data[EFUSE_CHANNEL_DI_LHS].protocol_state = PROTOCOL_STATE_OFF;
            retry_data[EFUSE_CHANNEL_DI_RHS].protocol_state = PROTOCOL_STATE_OFF;
            retry_data[EFUSE_CHANNEL_AIR].protocol_state    = PROTOCOL_STATE_OFF;
            break;
        case RETRY_PROTOCOL_AIR:
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, true);

            retry_data[EFUSE_CHANNEL_AIR].protocol_state   = PROTOCOL_STATE_OFF;
            retry_data[EFUSE_CHANNEL_LVPWR].protocol_state = PROTOCOL_STATE_OFF;
            break;
        case RETRY_PROTOCOL_AUX:
            retry_data[EFUSE_CHANNEL_AUX].protocol_state = PROTOCOL_STATE_OFF;
            break;
        case RETRY_PROTOCOL_FANS:
            retry_data[EFUSE_CHANNEL_FAN].protocol_state = PROTOCOL_STATE_OFF;
            break;
        case RETRY_PROTOCOL_EMETER:
            retry_data[EFUSE_CHANNEL_FAN].protocol_state = PROTOCOL_STATE_OFF;
            break;
        case RETRY_PROTOCOL_DRS:
            retry_data[EFUSE_CHANNEL_FAN].protocol_state = PROTOCOL_STATE_OFF;
            break;
        default:
            break;
    }
}
