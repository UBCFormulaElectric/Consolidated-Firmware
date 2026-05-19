#include <cstddef>

#include "app_states.hpp"
#include "app_precharge.hpp"
// #include "app_segments.hpp"
#include "io_irs.hpp"
#include "io_faultLatch.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

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
        // const bool precharge_ok = !app_precharge_limitExceeded(); // Optional condition
        if (not app::can_alerts::AnyBoardHasFault())
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