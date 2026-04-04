#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_canAlerts.hpp"

using namespace app::can_utils;

namespace app::states
{

namespace initState
{
static void runOnEntry(void)
{
    app::can_tx::VC_State_set(VCState::VC_INIT_STATE);
}

static void runOnTick100Hz(void)
{
    const ContactorState air_minus_closed = app::can_rx::BMS_IrPositive_get();
    if (air_minus_closed == ContactorState::CONTACTOR_STATE_CLOSED)
    {
        app::StateMachine::set_next_state(&inverterOn_state);
    }
}

static void runOnExit(void) {}
}

State init_state = { .name              = "INIT",
                     .run_on_entry      = initState::runOnEntry,
                     .run_on_tick_1Hz   = nullptr,
                     .run_on_tick_100Hz = initState::runOnTick100Hz,
                     .run_on_exit       = initState::runOnExit };
} // namespace app::states