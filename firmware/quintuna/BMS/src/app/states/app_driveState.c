#include "states/app_states.h"

#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_timer.h"
#include "io_irs.h"

#define AIR_N_DEBOUNCE_PERIOD (200) // ms

static TimerChannel debounce_timer;

static void driveStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_DRIVE_STATE);
    app_timer_init(&debounce_timer, AIR_N_DEBOUNCE_PERIOD);
}

static void driveStateRunOnTick100Hz(void) {}

static void driveStateRunOnExit(void)
{
    // IR+ opens upon exiting drive state
    io_irs_setPositive(IRS_OPEN);
}

const State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = driveStateRunOnEntry,
    .run_on_tick_100Hz = driveStateRunOnTick100Hz,
    .run_on_exit       = driveStateRunOnExit,
};