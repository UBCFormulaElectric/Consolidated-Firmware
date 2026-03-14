#include <cstddef>

#include "app_states.hpp"
#include "app_precharge.hpp"
// #include "app_segments.hpp"
#include "io_irs.hpp"
#include "io_faultLatch.hpp"
#include "app_canTx.hpp"

namespace app::states
{

namespace faultState
{
    static void runOnEntry()
    {
        app::can_tx::BMS_State_set(app::can_utils::BmsState::BMS_FAULT_STATE);

        // Always open AIR+ when entering fault state to isolate high voltage.
        io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    }

    static void runOnTick100Hz()
    {
#ifdef TARGET_HV_SUPPLY
        const bool acc_fault_cleared = true;
#else
        // TODO: Change back if we ever get to segments again
        // const bool acc_fault_cleared = !app::segments::checkFaults();
        const bool acc_fault_cleared = false;
#endif

        // const bool precharge_ok = !app_precharge_limitExceeded(); // Optional condition

        const bool bms_fault_cleared =
            (io::faultLatch::getLatchedStatus(&io::faultLatch::bms_ok_latch) == io::faultLatch::FaultLatchState::OK);

        if (acc_fault_cleared && bms_fault_cleared)
        {
            app::StateMachine::set_next_state(&init_state);
        }
    }
} // namespace faultState

const ::app::State fault_state = {
    .name              = "FAULT",
    .run_on_entry      = faultState::runOnEntry,
    .run_on_tick_1Hz   = nullptr,
    .run_on_tick_100Hz = faultState::runOnTick100Hz,
    .run_on_exit       = nullptr,
};
} // namespace app::states