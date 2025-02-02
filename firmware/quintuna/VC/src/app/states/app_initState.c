#include "app_initState.h"
#include "app_allStates.h"
#include "app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static void initStateRunOnEntry(void) {}
static void initStateRunOnTick1Hz(void) {}
static void initStateRunOnTick100Hz(void) {}
static void initStateRunOnExit(void) {}

const State *app_initState_get(void)
{
    static State init_state = { .name              = "INIT",
                                .run_on_entry      = initStateRunOnEntry,
                                .run_on_tick_1Hz   = initStateRunOnTick1Hz,
                                .run_on_tick_100Hz = initStateRunOnTick100Hz,
                                .run_on_exit       = initStateRunOnExit };

    return &init_state;
}
