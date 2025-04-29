#include "app_allStates.h"
#include "app_bmsOnState.h"

static void bmsOnStateRunOnEntry(void) {}
static void bmsOnStateRunOnTick1Hz(void) {}
static void bmsOnStateRunOnTick100Hz(void) {}
static void bmsOnStateRunOnExit(void) {}


State bmsOn_state = { .name              = "BMS ON",
                                .run_on_entry      = bmsOnStateRunOnEntry,
                                .run_on_tick_1Hz   = bmsOnStateRunOnTick1Hz,
                                .run_on_tick_100Hz = bmsOnStateRunOnTick100Hz,
                                .run_on_exit       = bmsOnStateRunOnExit };
