#include "app_states.h"

static void prechargeStateRunOnEntry() {}
static void prechargeStateRunOnTick100Hz() {}
static void prechargeStateRunOnExit() {}

const State precharge_state = {
    .name              = "PRECHARGE",
    .run_on_entry      = prechargeStateRunOnEntry,
    .run_on_tick_100Hz = prechargeStateRunOnTick100Hz,
    .run_on_exit       = prechargeStateRunOnExit,
};