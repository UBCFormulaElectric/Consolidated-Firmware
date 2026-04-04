#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace bmsOnStates
{

static void runOnEntry(void)
{
    app::can_tx::VC_State_set(VCState::VC_BMS_ON_STATE);
}

static void runOnTick100Hz(void)
{
    if (app::can_rx::BMS_State_get() == BmsState::BMS_DRIVE_STATE)
    {
        app::StateMachine::set_next_state(&pcmOn_state);
    }
}

static void runOnExit(void) {}
} // namespace bmsOnStates


State bmsOn_state = { .name              = "BMS ON",
                      .run_on_entry      = bmsOnStates::runOnEntry,
                      .run_on_tick_1Hz   = nullptr,
                      .run_on_tick_100Hz = bmsOnStates::runOnTick100Hz,
                      .run_on_exit       = bmsOnStates::runOnExit };
} // namespace app::states
