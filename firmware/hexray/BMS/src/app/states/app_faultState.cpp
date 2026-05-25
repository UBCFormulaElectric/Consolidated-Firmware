#include "app_states.hpp"
#include "app_precharge.hpp"
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
        // bms_ok_latch is hardware-latched: once tripped it stays FAULT until externally
        // reset, and alerts::tick() in jobs.cpp re-trips it every cycle a segment fault
        // is active. So the latch returning OK is the single source of truth for "the
        // segment/ADBMS fault has cleared and the operator has acknowledged it."
        if (bms_ok_latch.getLatchedStatus() == io::FaultLatch::FaultLatchState::OK)
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
