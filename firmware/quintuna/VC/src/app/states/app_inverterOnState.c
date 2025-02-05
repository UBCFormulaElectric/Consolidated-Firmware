#include "app_initState.h"
#include "app_allStates.h"
#include "app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static void inverterOnStateRunOnEntry(void) {}
static void inverterOnStateRunOnTick1Hz(void) {}
static void inverterOnStateRunOnTick100Hz(void) {}
static void inverterOnStateRunOnExit(void) {}

const State *app_inverterOnState_get(void)
{
    static State inverterOn_state = { .name              = "INVERTER ON",
                                .run_on_entry      = inverterOnStateRunOnEntry,
                                .run_on_tick_1Hz   = inverterOnStateRunOnTick1Hz,
                                .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
                                .run_on_exit       = inverterOnStateRunOnExit };

    return &inverterOn_state;
}
