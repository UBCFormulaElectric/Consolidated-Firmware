#include <math.h>
#include "stddef.h"

#include "states/app_initState.h"
#include "states/app_allStates.h"
#include "app_utils.h"

static void initStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();
}

const State *app_initState_get()
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void initStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    app_allStates_runOnTick100Hz(state_machine);

    if (App_CanRx_BMS_State_Get() == BMS_INVERTER_ON_STATE)
    {
        App_SharedStateMachine_SetNextState(state_machine, app_driveState_get());
    }
}

static void initStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *app_initState_get()
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = NULL,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &init_state;
}
