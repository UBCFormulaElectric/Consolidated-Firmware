#include <math.h>

#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "states/app_driveState.h"
#include "states/app_inverterOnState.h"

#ifdef TARGET_EMBEDDED
#include "io_canTx.h"
#endif

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_powerManager.h"
#include "app_torqueVectoring.h"
#include "app_faultCheck.h"
#include "app_regen.h"
#include "app_units.h"
#include "app_signal.h"
#include "app_utils.h"

static void driveStateRunOnEntry(void) {}

static void driveStateRunOnTick1Hz(void) {}

static void driveStateRunOnTick100Hz(void) {}

static void driveStateRunOnExit(void) {}

const State *app_driveState_get(void)
{
    static State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };

    return &drive_state
}
