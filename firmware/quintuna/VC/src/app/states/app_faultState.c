#include "app_states.h"

static void faultStateRunOnEntry(void) {}
static void faultRunOnTick1Hz(void) {}
static void faultRunOnTick100Hz(void) {}
static void faultRunOnExit(void) {}

State fault_state = { .name              = "FAULT",
                      .run_on_entry      = faultStateRunOnEntry,
                      .run_on_tick_1Hz   = faultRunOnTick1Hz,
                      .run_on_tick_100Hz = faultRunOnTick100Hz,
                      .run_on_exit       = faultRunOnExit };
