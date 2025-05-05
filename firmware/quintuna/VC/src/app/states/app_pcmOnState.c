#include "app_states.h"

static void pcmOntateRunOnEntry(void) {}
static void pcmOnStateRunOnTick1Hz(void) {}
static void pcmOnStateRunOnTick100Hz(void) {}
static void pcmOnStateRunOnExit(void) {}

State pcmOn_state = { .name              = "PCM ON",
                      .run_on_entry      = pcmOntateRunOnEntry,
                      .run_on_tick_1Hz   = pcmOnStateRunOnTick1Hz,
                      .run_on_tick_100Hz = pcmOnStateRunOnTick100Hz,
                      .run_on_exit       = pcmOnStateRunOnExit };