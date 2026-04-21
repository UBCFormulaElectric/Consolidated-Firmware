#include "app_stateMachine.hpp"
#include "torque_vectoring/driving_algorithm.hpp"
#include "app_bspdWarning.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

static bool driveStatePassPreCheck()
{
// TODO:
// check inverter warnings
// check board warnings
// check possibly other faults 
// check switches and debounce them
// check bspd
// handle state transitions away from drive state
// is this really needed ngl this could be consolidated into 100Hz tick
return true;
}

static void driveStateRunOnEntry()
{
// TODO:
// enable inverters
}

static void driveStateRunOnTick100Hz(void)
{
// TODO:
    if (!driveStatePassPreCheck())
        return;
// run driving algo
}

static void driveStateRunOnExit(void)
{
// TODO:
// disable inverters
}

app::State drive_state = { .name              = "DRIVE",
                           .run_on_entry      = driveStateRunOnEntry,
                           .run_on_tick_1Hz   = nullptr,
                           .run_on_tick_100Hz = driveStateRunOnTick100Hz,
                           .run_on_exit       = driveStateRunOnExit };