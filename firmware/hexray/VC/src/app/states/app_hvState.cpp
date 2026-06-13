#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_startSwitch.hpp"
#include "app_powerManager.hpp"
#include "io_log.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace hvState
{
    static void runOnEntry()
    {
        LOG_INFO("entering hv state!");
        static constexpr powerManager::Efuses<powerManager::EfuseConfig> power_manager_state = {
            .front_efuse     = { true, 0, 5 },    // front
            .rsm_efuse       = { true, 0, 5 },    // rsm
            .bms_efuse       = { true, 0, 5 },    // bms
            .dam_efuse       = { true, 0, 5 },    // dam
            .f_inv_efuse     = { true, 0, 5 },    // f_inv
            .r_inv_efuse     = { true, 0, 5 },    // r_inv
            .r_rad_fan_efuse = { false, 200, 5 }, // r_rad_fan
            .l_rad_fan_efuse = { false, 200, 5 }, // l_rad_fan
            .rr_pump_efuse   = { false, 200, 5 }, // rr_pump
            .rl_pump_efuse   = { false, 200, 5 }, // rl_pump

        };
        app::powerManager::updateConfig(power_manager_state);

        can_tx::VC_State_set(VCState::VC_HV_ON_STATE);
    }

    static void runOnTick100Hz()
    {
        if (can_rx::BMS_State_get() == BmsState::BMS_INIT_STATE)
        {
            StateMachine::set_next_state(&init_state);
            return;
        }
        // TODO check inverter preconditions, return to hv init if not fulfilled
        // Conditions for entering drive state: minimum 50% braking and start switch
        // TODO: change this to a faster method after fault recovery
        if (const bool is_brake_actuated = can_rx::FSM_BrakeActuated_get();
            is_brake_actuated && startSwitch::hasRisingEdge())
        {
            // Transition to drive state when start-up conditions are passed (see
            // EV.10.4.3):
            StateMachine::set_next_state(&drive_state);
        }
    }

    static void runOnExit()
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
