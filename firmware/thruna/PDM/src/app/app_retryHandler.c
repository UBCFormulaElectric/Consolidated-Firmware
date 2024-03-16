#include <stdbool.h>

#include "app_retryHandler.h"
#include "io_efuse.h"
#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "app_canRx.h"
#include "app_canUtils.h"
#include "app_retryHandler.h"
#include "app_powerManager.h"

void retry_handler_start(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[])
{
    switch (protocol)
    {
        // turn off lv and inverters
        case RETRY_PROTOCOL_LV:
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, false);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, false);

            retry_data[EFUSE_CHANNEL_LVPWR].retry_state  = RETRY_STATE_RUNNING;
            retry_data[EFUSE_CHANNEL_DI_LHS].retry_state = RETRY_STATE_WAITING;
            retry_data[EFUSE_CHANNEL_DI_RHS].retry_state = RETRY_STATE_WAITING;
            break;
        // turn off inverters and aux
        case RETRY_PROTOCOL_DI_LHS:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, false);

            retry_data[EFUSE_CHANNEL_DI_LHS].retry_state = RETRY_STATE_RUNNING;
            retry_data[EFUSE_CHANNEL_DI_RHS].retry_state = RETRY_STATE_WAITING;
            retry_data[EFUSE_CHANNEL_AIR].retry_state    = RETRY_STATE_WAITING;
            break;
        // turn off inverters and aux
        case RETRY_PROTOCOL_DI_RHS:
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, false);
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, false);

            retry_data[EFUSE_CHANNEL_DI_LHS].retry_state = RETRY_STATE_WAITING;
            retry_data[EFUSE_CHANNEL_DI_RHS].retry_state = RETRY_STATE_RUNNING;
            retry_data[EFUSE_CHANNEL_AIR].retry_state    = RETRY_STATE_WAITING;
            break;
        // turn off air and lv, go to init state
        case RETRY_PROTOCOL_AIR:
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, false);
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, false);

            app_stateMachine_setNextState(app_initState_get());

            retry_data[EFUSE_CHANNEL_AIR].retry_state   = RETRY_STATE_RUNNING;
            retry_data[EFUSE_CHANNEL_LVPWR].retry_state = RETRY_STATE_WAITING;
            break;
        // go back to init state
        case RETRY_PROTOCOL_AUX:
            io_efuse_setChannel(EFUSE_CHANNEL_AUX, false);
            app_stateMachine_setNextState(app_initState_get());
            retry_data[EFUSE_CHANNEL_AUX].retry_state = RETRY_STATE_RUNNING;
            break;
        // turn off fans
        case RETRY_PROTOCOL_FANS:
            io_efuse_setChannel(EFUSE_CHANNEL_FAN, false);
            retry_data[EFUSE_CHANNEL_FAN].retry_state = RETRY_STATE_RUNNING;
            break;
        // turn off emeter
        case RETRY_PROTOCOL_EMETER:
            io_efuse_setChannel(EFUSE_CHANNEL_EMETER, false);
            retry_data[EFUSE_CHANNEL_EMETER].retry_state = RETRY_STATE_RUNNING;
            break;
        // turn off drs
        case RETRY_PROTOCOL_DRS:
            io_efuse_setChannel(EFUSE_CHANNEL_EMETER, false);
            retry_data[EFUSE_CHANNEL_DRS].retry_state = RETRY_STATE_RUNNING;
        default:
            break;
    }
}

void retry_handler_recover(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[])
{
    switch (protocol)
    {
        case RETRY_PROTOCOL_LV:
            io_efuse_standbyReset(EFUSE_CHANNEL_LVPWR);
            io_efuse_standbyReset(EFUSE_CHANNEL_DI_LHS);
            io_efuse_standbyReset(EFUSE_CHANNEL_DI_RHS);

            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, true);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, true);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, true);

            retry_data[EFUSE_CHANNEL_LVPWR].retry_state  = RETRY_STATE_OFF;
            retry_data[EFUSE_CHANNEL_DI_LHS].retry_state = RETRY_STATE_OFF;
            retry_data[EFUSE_CHANNEL_DI_RHS].retry_state = RETRY_STATE_OFF;
            break;
        case RETRY_PROTOCOL_DI_LHS:
        case RETRY_PROTOCOL_DI_RHS:
            io_efuse_standbyReset(EFUSE_CHANNEL_DI_LHS);
            io_efuse_standbyReset(EFUSE_CHANNEL_DI_RHS);
            io_efuse_standbyReset(EFUSE_CHANNEL_AIR);

            io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, true);
            io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, true);
            io_efuse_setChannel(EFUSE_CHANNEL_AIR, true);

            retry_data[EFUSE_CHANNEL_DI_LHS].retry_state = RETRY_STATE_OFF;
            retry_data[EFUSE_CHANNEL_DI_RHS].retry_state = RETRY_STATE_OFF;
            retry_data[EFUSE_CHANNEL_AIR].retry_state    = RETRY_STATE_OFF;
            break;
        case RETRY_PROTOCOL_AIR:
            io_efuse_standbyReset(EFUSE_CHANNEL_AIR);
            io_efuse_standbyReset(EFUSE_CHANNEL_LVPWR);

            io_efuse_setChannel(EFUSE_CHANNEL_AIR, true);
            io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, true);

            app_stateMachine_setNextState(app_initState_get());

            retry_data[EFUSE_CHANNEL_AIR].retry_state   = RETRY_STATE_OFF;
            retry_data[EFUSE_CHANNEL_LVPWR].retry_state = RETRY_STATE_OFF;
            break;
        case RETRY_PROTOCOL_AUX:
            io_efuse_standbyReset(EFUSE_CHANNEL_AUX);
            io_efuse_setChannel(EFUSE_CHANNEL_AUX, true);
            retry_data[EFUSE_CHANNEL_AUX].retry_state = RETRY_STATE_OFF;
            break;
        case RETRY_PROTOCOL_FANS:
            io_efuse_standbyReset(EFUSE_CHANNEL_FAN);
            io_efuse_setChannel(EFUSE_CHANNEL_FAN, true);
            retry_data[EFUSE_CHANNEL_FAN].retry_state = RETRY_STATE_OFF;
            break;
        case RETRY_PROTOCOL_EMETER:
            io_efuse_standbyReset(EFUSE_CHANNEL_EMETER);
            io_efuse_setChannel(EFUSE_CHANNEL_EMETER, true);
            retry_data[EFUSE_CHANNEL_FAN].retry_state = RETRY_STATE_OFF;
            break;
        case RETRY_PROTOCOL_DRS:
            io_efuse_standbyReset(EFUSE_CHANNEL_DRS);
            io_efuse_setChannel(EFUSE_CHANNEL_DRS, true);
            retry_data[EFUSE_CHANNEL_FAN].retry_state = RETRY_STATE_OFF;
            break;
        default:
            break;
    }
}
