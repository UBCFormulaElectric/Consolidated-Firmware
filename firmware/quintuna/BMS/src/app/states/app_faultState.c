#include "app_states.h"
#include "io_irs.h"

static void app_faultStateRunOnEntry()
{
    io_irs_openPositive();
    io_irs_openPrecharge();
}
static void app_faultStateRunOnTick1Hz() {}
static void app_faultStateRunOnTick100Hz() {}
static void app_faultStateRunOnExit() {}

const State fault_state = {
    .name              = "FAULT",
    .run_on_entry      = app_faultStateRunOnEntry,
    .run_on_tick_1Hz   = app_faultStateRunOnTick1Hz,
    .run_on_tick_100Hz = app_faultStateRunOnTick100Hz,
    .run_on_exit       = app_faultStateRunOnExit,
};