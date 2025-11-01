#include <cstddef>

#include "app_states.hpp"

extern "C" {
#include "app_precharge.h"
#include "app_segments.h"
#include "io_irs.h"
#include "io_faultLatch.h"
#include "app_canTx.h"
}

namespace app::states::faultState
{

static void runOnEntry()
{
    app_canTx_BMS_State_set(BMS_FAULT_STATE);

    // Always open AIR+ when entering fault state to isolate high voltage.
    io_irs_setPositive(CONTACTOR_STATE_OPEN);
}

static void runOnTick100Hz()
{
#ifdef TARGET_HV_SUPPLY
    const bool acc_fault_cleared = true;
#else
    const bool acc_fault_cleared = !app_segments_checkFaults();
#endif

    // const bool precharge_ok = !app_precharge_limitExceeded(); // Optional condition

    const bool bms_fault_cleared =
        (io_faultLatch_getLatchedStatus(&bms_ok_latch) == FAULT_LATCH_OK);

    if (acc_fault_cleared && bms_fault_cleared)
    {
        app::StateMachine::set_next_state(&init_state);
    }
}

} // namespace app::states::faultState

const State fault_state = {
    .name              = "FAULT",
    .run_on_entry      = app::states::faultState::runOnEntry,
    .run_on_tick_100Hz = app::states::faultState::runOnTick100Hz,
    .run_on_exit       = nullptr,
};