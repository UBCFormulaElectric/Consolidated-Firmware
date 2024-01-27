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
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = NULL,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &init_state;
}
