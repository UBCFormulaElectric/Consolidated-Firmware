#include "app_hvState.h"

static void hvStateRunOnEntry(void) {}
static void hvStateRunOnTick1Hz(void) {}
static void hvStateRunOnTick100Hz(void) {}
static void hvStateRunOnExit(void) {}

const State *app_hvState_get(void)
{
    static State hv_state = { .name              = "HV",
                                .run_on_entry      = hvStateRunOnEntry,
                                .run_on_tick_1Hz   = hvStateRunOnTick1Hz,
                                .run_on_tick_100Hz = hvStateRunOnTick100Hz,
                                .run_on_exit       = hvStateRunOnExit };

    return &hv_state;
}
