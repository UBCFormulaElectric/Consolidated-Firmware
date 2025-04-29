#include "app_inverterOnState.h"

static void inverterOnStateRunOnEntry(void) {}
static void inverterOnStateRunOnTick1Hz(void) {}
static void inverterOnStateRunOnTick100Hz(void) {}
static void inverterOnStateRunOnExit(void) {}

State inverterOn_state = { .name              = "INVERTER ON",
                                      .run_on_entry      = inverterOnStateRunOnEntry,
                                      .run_on_tick_1Hz   = inverterOnStateRunOnTick1Hz,
                                      .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
                                      .run_on_exit       = inverterOnStateRunOnExit };