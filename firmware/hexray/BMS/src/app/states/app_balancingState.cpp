#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "io_irs.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

namespace app::balancingState
{

static void balancingStateRunOnEntry()
{
    app::can_tx::BMS_State_set(app::can_utils::BmsState::BMS_BALANCING_STATE);
}

static void balancingStateRunOnTick100Hz()
{
    const bool air_negative_open = io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;
    const bool stopped_requesting_balance = !app::can_rx::Debug_CellBalancingRequest_get();
    if (air_negative_open || stopped_requesting_balance)
    {
        app::StateMachine::set_next_state(&app::states::init_state);
    }
}

static void balancingStateRunOnExit()
{
    app::can_rx::Debug_CellBalancingRequest_update(false);
}
} // namespace app::balancingState

const app::State balancing_state = {
    .name              = "BALANCING",
    .run_on_entry      = app::balancingState::balancingStateRunOnEntry,
    .run_on_tick_1Hz   = nullptr,
    .run_on_tick_100Hz = app::balancingState::balancingStateRunOnTick100Hz,
    .run_on_exit       = app::balancingState::balancingStateRunOnExit,
};