#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "app_canUtils.hpp"
#include "app_startSwitch.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace hvState
{
static void runOnEntry(void)
{
    app::can_tx::VC_State_set(VCState::VC_HV_ON_STATE);
}

static void runOnTick100Hz(void)
{
    // Conditions for entering drive state: minimum 50% braking and start switch
    // TODO: change this to a faster method after fault recovery
    const bool is_brake_actuated = app::can_rx::FSM_BrakeActuated_get();
    if (is_brake_actuated && app::startSwitch::hasRisingEdge())
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):
        app::StateMachine::set_next_state(&drive_state);
    }
}

static void runOnExit(void){}
} // namespace hvState

State hv_state = { .name              = "HV",
                   .run_on_entry      = hvState::runOnEntry,
                   .run_on_tick_1Hz   = nullptr,
                   .run_on_tick_100Hz = hvState::runOnTick100Hz,
                   .run_on_exit       = hvState::runOnExit };


} // namespace app::states
