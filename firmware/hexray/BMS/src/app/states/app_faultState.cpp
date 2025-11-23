#include <cstddef>

#include "app_states.hpp"
#include "app_precharge.hpp"
// #include "app_segments.hpp"
#include "io_irs.hpp"
#include "io_faultLatch.hpp"

extern "C"
{
#include "app_canTx.h"
}

namespace app::states::faultState
{

static void runOnEntry()
{
    app_canTx_BMS_State_set(BMS_FAULT_STATE);

    // Always open AIR+ when entering fault state to isolate high voltage.
    io::irs::setPositive(CONTACTOR_STATE_OPEN);
}

static void runOnTick100Hz()
{
#ifdef TARGET_HV_SUPPLY
    const bool acc_fault_cleared = true;
#else
    const bool acc_fault_cleared = !app::segments::checkFaults();
#endif

    // const bool precharge_ok = !app_precharge_limitExceeded(); // Optional condition

    const bool bms_fault_cleared = (io::faultLatch::getLatchedStatus(&io::faultLatch::bms_ok_latch) == io::faultLatch::FaultLatchState::OK);

    if (acc_fault_cleared && bms_fault_cleared)
    {
        app::StateMachine::set_next_state(&init_state);
    }
}

} // namespace app::states::faultState

const app::State fault_state = {
    .name              = "FAULT",
    .run_on_entry      = app::states::faultState::runOnEntry,
    .run_on_tick_100Hz = app::states::faultState::runOnTick100Hz,
    .run_on_exit       = nullptr,
};