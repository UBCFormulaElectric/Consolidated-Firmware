#include "app_initState.h"
#include "app_allStates.h"
#include "app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static void hvInitStateRunOnEntry(void){}
static void hvInitStateRunOnTick1Hz(void){}
static void hvInitStateRunOnTick100Hz(void){}
static void hvInitStateRunOnExit(void) {}


const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "HV INIT",
        .run_on_entry      = hvInitStateRunOnEntry,
        .run_on_tick_1Hz   = hvInitStateRunOnTick1Hz,
        .run_on_tick_100Hz = hvInitStateRunOnTick100Hz,
        .run_on_exit = hvInitStateRunOnExit
    };

    return &init_state;
}
