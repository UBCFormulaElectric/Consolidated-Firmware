#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "io_log.hpp"
#include "app_canUtils.hpp"
#include "app_inverter.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace faultState
{

    static void runOnEntry(void)
    {
    }

    static void runOnTick100Hz(void) 
    {
    }

    static void runOnExit(void)
    {
    }
    
} // namespace fault

State fault_state = { .name = "Fault State",
                      .run_on_entry      = faultState::runOnEntry,
                      .run_on_tick_1Hz   = nullptr,
                      .run_on_tick_100Hz = faultState::runOnTick100Hz,
                      .run_on_exit       = faultState::runOnExit };

} // namespace app::states
