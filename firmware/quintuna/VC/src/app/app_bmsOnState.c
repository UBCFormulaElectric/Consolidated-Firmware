#include "app_initState.h"
#include "app_allStates.h"
#include "app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static void bmsOnStateRunOnEntry(void){}
static void bmsOnStateRunOnTick1Hz(void){}
static void bmsOnStateRunOnTick100Hz(void){}
static void bmsOnStateRunOnExit(void) {}


const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "BMS ON",
        .run_on_entry      = bmsOnStateRunOnEntry,
        .run_on_tick_1Hz   = bmsOnStateRunOnTick1Hz,
        .run_on_tick_100Hz = bmsOnStateRunOnTick100Hz,
        .run_on_exit = bmsOnStateRunOnExit
    };

    return &init_state;
}
