#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_segments.hpp"
#include "app_irs.hpp"

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
        const bool ir_negative_opened_debounced = app::irs::negativeOpenedDebounced();
        const bool balancing_enabled            = app::can_rx::Debug_CellBalancing_Request_get();

        if (balancing_enabled)
        {
            app::segments::balancing::tick();
        }

        if (!balancing_enabled)// TODO: add ir negative
        {
            app::StateMachine::set_next_state(&app::states::init_state);
        }
    }

    static void balancingStateRunOnExit()
    {
        app::segments::balancing::disable();
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
