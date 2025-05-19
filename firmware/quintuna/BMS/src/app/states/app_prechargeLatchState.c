#include "app_stateMachine.h"

static void app_prechargeLatchStateRunOnEntry()
{

}

static void app_prechargeLatchStateRunOnTick1Hz()
{

}

static void app_prechargeLatchStateRunOnTick100Hz()
{
    if(app_canRx_Debug_ClearChargerLatchedFault())
        app_stateMachine_setNextState(app_initState_get());
}

static void app_prechargeLatchStateRunOnExit()
{

}

const State *app_prechargeLatchState_get(void)
{
    static State precharge_latch_state = {
        .name               = "PRECHARGE LATCH",
        .run_on_entry       = app_prechargeLatchStateRunOnEntry,
        .run_on_tick_1Hz    = app_prechargeLatchStateRunOnTick1Hz,
        .run_on_tick_100Hz  = app_prechargeLatchStateRunOnTick100Hz,
        .run_on_exit        = app_prechargeLatchStateRunOnExit,
    };

    return &precharge_latch_state;
}