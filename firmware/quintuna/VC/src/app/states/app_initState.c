#include "app_initState.h"
#include "app_canUtils.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_stateMachine.h"
#include "app_inverterOnState.h"
#include "app_allStates.h"

static void initStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INIT_STATE);
}

static void initStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void initStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();

    bool air_minus_closed = app_canRx_BMS_AirNegative_get() == CONTACTOR_STATE_CLOSED;

    if (air_minus_closed)
    {
        app_stateMachine_setNextState(&inverterOn_state);
    }
}

State init_state = { .name              = "INIT",
                     .run_on_entry      = initStateRunOnEntry,
                     .run_on_tick_1Hz   = initStateRunOnTick1Hz,
                     .run_on_tick_100Hz = initStateRunOnTick100Hz };
