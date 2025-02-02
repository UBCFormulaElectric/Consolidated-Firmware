#include "app_initState.h"
#include "app_allStates.h"
#include "app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static void pcmOntateRunOnEntry(void){}
static void pcmOnStateRunOnTick1Hz(void){}
static void pcmOnStateRunOnTick100Hz(void){}
static void pcmOnStateRunOnExit(void) {}


const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "PCM ON",
        .run_on_entry      = pcmOntateRunOnEntry,
        .run_on_tick_1Hz   = pcmOnStateRunOnTick1Hz,
        .run_on_tick_100Hz = pcmOnStateRunOnTick100Hz,
        .run_on_exit = pcmOnStateRunOnExit
    };

    return &init_state;
}
