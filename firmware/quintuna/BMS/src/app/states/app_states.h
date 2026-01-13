#pragma once
#include "app_stateMachine.h"

extern const State init_state;
extern const State fault_state;

extern const State precharge_drive_state;
extern const State drive_state;

extern const State balancing_state;
extern const State precharge_latch_state;

// charge states
extern const State precharge_charge_state;
extern const State charge_state;
extern const State charge_init_state;
extern const State charge_fault_state;