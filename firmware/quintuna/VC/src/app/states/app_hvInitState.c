#include "app_states.h"

static void hvInitStateRunOnEntry(void) {}
static void hvInitStateRunOnTick1Hz(void) {}
static void hvInitStateRunOnTick100Hz(void) {}
static void hvInitStateRunOnExit(void) {}

State hvInit_state = { .name              = "HV INIT",
                       .run_on_entry      = hvInitStateRunOnEntry,
                       .run_on_tick_1Hz   = hvInitStateRunOnTick1Hz,
                       .run_on_tick_100Hz = hvInitStateRunOnTick100Hz,
                       .run_on_exit       = hvInitStateRunOnExit };
