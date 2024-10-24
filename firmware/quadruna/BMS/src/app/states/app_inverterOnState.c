#include "states/app_allStates.h"
#include "states/app_prechargeState.h"
#include "states/app_inverterOnState.h"
#include "app_utils.h"
#include "app_timer.h"

static void inverterOnStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_INVERTER_ON_STATE);
}

static void inverterOnStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void inverterOnStateRunOnTick100Hz(void)
{
    const bool ready_for_precharge = app_canRx_VC_isPrechargeReady_get();

    if (app_allStates_runOnTick100Hz())
    {
        if (ready_for_precharge)
        {
            app_stateMachine_setNextState(app_prechargeState_get());
        }
    }
}

const State *app_inverterOnState_get(void)
{
    static State inverter_state = {
        .name              = "INVERTER_ON",
        .run_on_entry      = inverterOnStateRunOnEntry,
        .run_on_tick_1Hz   = inverterOnStateRunOnTick1Hz,
        .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &inverter_state;
}
