#pragma once

#include "app_stateMachine.hpp"

namespace app::states
{
    // Initialization and fault states
    extern const State init_state;
    extern const State fault_state;

    // Driving states
    extern const State precharge_drive_state;
    extern const State drive_state;

    // Balancing and latch states
    extern const State balancing_state;
    extern const State precharge_latch_state;

    // Charging states
    extern const State precharge_charge_state;
    extern const State charge_state;
    extern const State charge_init_state;
    extern const State charge_fault_state;
} // namespace app::states