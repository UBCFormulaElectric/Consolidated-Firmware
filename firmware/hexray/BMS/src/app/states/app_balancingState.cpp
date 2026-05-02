#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "io_irs.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_segments.hpp"
namespace app::states
{

namespace balancingState
{

    static void balancingStateRunOnEntry()
    {
        app::can_tx::BMS_State_set(app::can_utils::BmsState::BMS_BALANCING_STATE);
        app::segments::balancingInit();

    }

    static void balancingStateRunOnTick100Hz()
    {
        const bool air_negative_open = io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;
        const bool balancing_enabled = app::can_rx::Debug_CellBalancingRequest_get();
        if (air_negative_open || !balancing_enabled)
        {
            app::StateMachine::set_next_state(&app::states::init_state);
        }
        app::segments::balancingEnable();
    }

    static void balancingStateRunOnExit()
    {
        app::segments::balancingDisable();
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
