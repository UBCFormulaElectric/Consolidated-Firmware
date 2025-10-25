#include "app_stateMachine.hpp"
#include "app_states.hpp"

extern "C" {
#include "io_irs.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_ltc6813.h"
}

static void balancingStateRunOnEntry() {
    app_canTx_BMS_State_set(BMS_BALANCING_STATE);
}

static void balancingStateRunOnTick100Hz() {
    const bool air_negative_open          = io_irs_negativeState() == CONTACTOR_STATE_OPEN;
    const bool stopped_requesting_balance = !app_canRx_Debug_CellBalancingRequest_get();
    if (air_negative_open || stopped_requesting_balance) {
        app::StateMachine::set_next_state(&app::states::init_state);
    }
}   

static void balancingStateRunOnExit() {
    app_canRx_Debug_CellBalancingRequest_update(false);
}

namespace app::balancingState {
    const State balancing_state = {
        .name              = "BALANCING",
        .run_on_entry      = balancingStateRunOnEntry,
        .run_on_tick_100Hz = balancingStateRunOnTick100Hz,
        .run_on_exit       = balancingStateRunOnExit,
    };
}