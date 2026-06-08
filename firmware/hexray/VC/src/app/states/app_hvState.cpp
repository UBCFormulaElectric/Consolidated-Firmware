#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "app_canUtils.hpp"
#include "app_startSwitch.hpp"
#include "app_powerManager.hpp"
#include "io_log.hpp"
#include "app_pumpControl.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace hvState
{
    static void runOnEntry(void)
    {
        LOG_INFO("entering hv state!");
        static constexpr app::powerManager::PowerManagerConfig power_manager_state = {
            .efuse_configs = { {
                { false, 200, 5 }, // rr_pump
                { true, 200, 5 },  // rl_pump
                { true, 200, 5 },  // r_rad_fan
                { true, 200, 5 },  // l_rad_fan
                { true, 0, 5 },    // f_inv
                { true, 0, 5 },    // r_inv
                { true, 0, 5 },    // rsm
                { true, 0, 5 },    // bms
                { true, 0, 5 },    // dam
                { true, 0, 5 },    // front
            } }
        };
        app::powerManager::updateConfig(power_manager_state);

        app::can_tx::VC_State_set(VCState::VC_HV_ON_STATE);
    }

    static void runOnTick100Hz(void)
    {
        if (app::can_rx::BMS_State_get() == app::can_utils::BmsState::BMS_INIT_STATE)
        {
            app::StateMachine::set_next_state(&init_state);
        }
        else 
        {
            // Conditions for entering drive state: minimum 50% braking and start switch
            // TODO: change this to a faster method after fault recovery
            app::pumpControl::MonitorPumps();
            const bool is_brake_actuated = app::can_rx::FSM_BrakeActuated_get();
            if (is_brake_actuated && app::startSwitch::hasRisingEdge())
            {
                // Transition to drive state when start-up conditions are passed (see
                // EV.10.4.3):
                app::StateMachine::set_next_state(&drive_state);
            }
        }
    }

    static void runOnExit(void)
    {
        LOG_INFO("exiting hv state!");
    }
} // namespace hvState

State hv_state = { .name              = "HV",
                   .run_on_entry      = hvState::runOnEntry,
                   .run_on_tick_1Hz   = nullptr,
                   .run_on_tick_100Hz = hvState::runOnTick100Hz,
                   .run_on_exit       = hvState::runOnExit };

} // namespace app::states
