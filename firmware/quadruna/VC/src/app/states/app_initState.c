#include <math.h>
#include "states/app_initState.h"
#include "states/app_allStates.h"

static void initStateRunOnEntry(void) {}

static void initStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void initStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();
}

static void initStateRunOnExit(void) {}

const State *app_initState_get()
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = initStateRunOnTick1Hz,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = initStateRunOnExit,
    };

    return &init_state;
}
