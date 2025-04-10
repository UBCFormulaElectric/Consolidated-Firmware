#include "app_hvInitState.h"

static void hvInitStateRunOnEntry(void) {}
static void hvInitStateRunOnTick1Hz(void) {}
static void hvInitStateRunOnTick100Hz(void) {}
static void hvInitStateRunOnExit(void) {}

const State *app_hvInitState_get(void)
{
    static State hvInit_state = { .name              = "HV INIT",
                                  .run_on_entry      = hvInitStateRunOnEntry,
                                  .run_on_tick_1Hz   = hvInitStateRunOnTick1Hz,
                                  .run_on_tick_100Hz = hvInitStateRunOnTick100Hz,
                                  .run_on_exit       = hvInitStateRunOnExit };

    return &hvInit_state;
}
