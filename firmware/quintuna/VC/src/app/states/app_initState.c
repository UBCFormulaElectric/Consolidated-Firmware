#include "app_initState.h"

static void initStateRunOnEntry(void) {}
static void initStateRunOnTick1Hz(void) {}
static void initStateRunOnTick100Hz(void) {}
static void initStateRunOnExit(void) {}

State init_state = { .name              = "INIT",
                     .run_on_entry      = initStateRunOnEntry,
                     .run_on_tick_1Hz   = initStateRunOnTick1Hz,
                     .run_on_tick_100Hz = initStateRunOnTick100Hz,
                     .run_on_exit       = initStateRunOnExit };
