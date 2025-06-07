#include "app_stateMachine.h"
#include "app_prechargeChargeState.h"
#include "io_irs.h"
#include "io_charger.h"
#include "stdio.h"
#include "app_canRx.h"

static void app_initStateRunOnEntry() {}

static void app_initStateRunOnTick1Hz() {}

static void app_initStateRunOnTick100Hz()
{
    const bool is_irs_negative_closed    = io_irs_isNegativeClosed();
    const bool external_charging_request = app_canRx_Debug_StartCharging_get();
    const bool is_charger_connected      = (io_charger_getConnectionStatus() == EVSE_CONNECTED || WALL_CONNECTED);

    if (is_irs_negative_closed && external_charging_request && is_charger_connected)
    {
        app_stateMachine_setNextState(app_prechargeChargeState_get());
    }
}

const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = app_initStateRunOnEntry,
        .run_on_tick_1Hz   = app_initStateRunOnTick1Hz,
        .run_on_tick_100Hz = app_initStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &init_state;
}