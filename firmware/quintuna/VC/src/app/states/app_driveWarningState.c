#include "app_initState.h"
#include "app_allStates.h"
#include "app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static void driveWarningStateRunOnEntry(void) {}
static void driveWarningStateRunOnTick1Hz(void) {}
static void driveWarningStateRunOnTick100Hz(void) {}
static void driveWarningStateRunOnExit(void) {}

const State *app_driveWarningState_get(void)
{
    static State driveWarning_state = { .name              = "DRIVE WARNING",
                                .run_on_entry      = driveWarningStateRunOnEntry,
                                .run_on_tick_1Hz   = driveWarningStateRunOnTick1Hz,
                                .run_on_tick_100Hz = driveWarningStateRunOnTick100Hz,
                                .run_on_exit       = driveWarningStateRunOnExit };

    return &driveWarning_state;
}
