#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "io_log.hpp"
#include "app_canUtils.hpp"
#include "app_inverter.hpp"
#include "app_powerManager.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace faultState
{
    static void runOnEntry(void)
    {
        static const app::powerManager::PowerManagerConfig power_manager_state = { .efuse_configs = { {
                                                                                       { false, 200, 5 }, // rr_pump
                                                                                       { false, 200, 5 }, // rl_pump
                                                                                       { false, 200, 5 }, // r_rad_fan
                                                                                       { false, 200, 5 }, // l_rad_fan
                                                                                       { false, 0, 5 },   // f_inv
                                                                                       { false, 0, 5 },   // r_inv
                                                                                       { true, 0, 5 },    // rsm
                                                                                       { true, 0, 5 },    // bms
                                                                                       { true, 0, 5 },    // dam
                                                                                       { true, 0, 5 },    // front
        } } };
        app::powerManager::updateConfig(power_manager_state);
        app::can_tx::VC_State_set(VCState::VC_FAULT_STATE);
    }

    static void runOnTick100Hz(void)
    {
        const bool fault_cleared = !app::can_alerts::AnyBoardHasFault();
        if (fault_cleared)
        {
            app::StateMachine::set_next_state(&init_state);
        }
    }

    static void runOnExit(void) {}

} // namespace faultState

State fault_state = { .name              = "Fault State",
                      .run_on_entry      = faultState::runOnEntry,
                      .run_on_tick_1Hz   = nullptr,
                      .run_on_tick_100Hz = faultState::runOnTick100Hz,
                      .run_on_exit       = faultState::runOnExit };

} // namespace app::states
