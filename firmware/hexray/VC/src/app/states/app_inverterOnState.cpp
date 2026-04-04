#include "app_stateMachine.hpp"
#include "app_canTx.hpp"
#include "app_states.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"


using namespace app::can_utils;
namespace app::states
{
namespace inverterOnState
{
static void runOnEntry(void)
{
    app::can_tx::VC_State_set(VCState::VC_INVERTER_ON_STATE);
}

static void runOnTick100Hz(void)
{
    const bool inverters_bsystemReady = app::can_rx::INVFL_bSystemReady_get() && app::can_rx::INVFR_bSystemReady_get() &&
                                       app::can_rx::INVRL_bSystemReady_get() && app::can_rx::INVRR_bSystemReady_get();

    if (inverters_bsystemReady)
    {   
        app::StateMachine::set_next_state(&bmsOn_state);
    }

} 

static void runOnExit(void) {}
}

State inverterOn_state = { .name              = "INVERTER ON",
                           .run_on_entry      = inverterOnState::runOnEntry,
                           .run_on_tick_1Hz   = nullptr,
                           .run_on_tick_100Hz = inverterOnState::runOnTick100Hz,
                           .run_on_exit       = inverterOnState::runOnExit };
}