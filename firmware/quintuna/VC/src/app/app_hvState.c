#include "app_initState.h"
#include "app_allStates.h"
#include "app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static void hvStateRunOnEntry(void){}
static void hvStateRunOnTick1Hz(void){}
static void hvStateRunOnTick100Hz(void){}
static void hvStateRunOnExit(void) {}


const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "HV",
        .run_on_entry      = hvStateRunOnEntry,
        .run_on_tick_1Hz   = hvStateRunOnTick1Hz,
        .run_on_tick_100Hz = hvStateRunOnTick100Hz,
        .run_on_exit = hvStateRunOnExit
    };

    return &init_state;
}
