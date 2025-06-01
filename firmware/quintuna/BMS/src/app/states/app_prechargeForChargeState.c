#include "states/app_prechargeForChargeState.h"
#include <stddef.h>
#include "io_irs.h"
#include "states/app_chargeState.h"
#include "app_precharge.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "states/app_faultState.h"
#include "states/app_initState.h"
#include "app_canUtils.h"

static void runOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_FOR_CHARGE_STATE);

    // Close precharge relay.
    io_irs_closePrecharge();
    app_precharge_restart();
}

static void runOnTick1Hz(void) {}

static void runOnTick100Hz(void)
{
    const bool           precharge_for_charging = true;
    const PrechargeState state                  = app_precharge_poll(precharge_for_charging);

    if (state == PRECHARGE_STATE_FAILED_CRITICAL)
    {
        app_stateMachine_setNextState(app_faultState_get());
    }
    else if (state == PRECHARGE_STATE_FAILED)
    {
        app_stateMachine_setNextState(app_initState_get());
    }
    else if (state == PRECHARGE_STATE_SUCCESS)
    {
        app_stateMachine_setNextState(app_chargeState_get());
    }
}

static void runOnExit(void)
{
    // If we entered precharge to charge, disable this request on exit.
    app_canRx_Debug_StartCharging_update(false);

    // Open precharge relay.
    io_irs_openPrecharge();
}

const State *app_prechargeForChargeState_get(void)
{
    static State pre_charge_for_charge_state = {
        .name              = "PRECHARGE_CHARGE",
        .run_on_entry      = runOnEntry,
        .run_on_tick_1Hz   = runOnTick1Hz,
        .run_on_tick_100Hz = runOnTick100Hz,
        .run_on_exit       = runOnExit,
    };

    return &pre_charge_for_charge_state;
}
