#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_segments.hpp"
#include "app_irs.hpp"
#include "io_irs.hpp"

namespace app::states
{
namespace balancingState
{

    static void balancingStateRunOnEntry()
    {
        app::can_tx::BMS_State_set(app::can_utils::BmsState::BMS_BALANCING_STATE);
        app::segments::balancing::init();
    }

    static void balancingStateRunOnTick100Hz()
    {
        const bool ir_negative_open =
            (io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
        const bool balancing_enabled = app::can_rx::Debug_CellBalancing_Request_get();

        if (balancing_enabled && !ir_negative_open)
        {
            app::segments::balancing::tick();
        }
        else
        {
            app::StateMachine::set_next_state(&app::states::init_state);
        }
    }

    static void balancingStateRunOnExit()
    {
        app::segments::balancing::disable();
        // Clear the balancing request so we don't immediately re-enter balancing from init.
        app::can_rx::Debug_CellBalancing_Request_update(false);
    }
} // namespace balancingState

const ::app::State balancing_state = {
    .name              = "BALANCING",
    .run_on_entry      = balancingState::balancingStateRunOnEntry,
    .run_on_tick_1Hz   = nullptr,
    .run_on_tick_100Hz = balancingState::balancingStateRunOnTick100Hz,
    .run_on_exit       = balancingState::balancingStateRunOnExit,
};
} // namespace app::states
