#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "app_canUtils.hpp"
#include "app_inverter.hpp"
#include "app_powerManager.hpp"
#include "io_log.hpp"
#include "io_pcm.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace faultState
{
    static void runOnEntry()
    {
        io::pcm::set(false);
        static constexpr powerManager::Efuses<powerManager::EfuseConfig> power_manager_state = {
            .front_efuse     = { true, 0, 5 },    // front
            .rsm_efuse       = { true, 0, 5 },    // rsm
            .bms_efuse       = { true, 0, 5 },    // bms
            .dam_efuse       = { true, 0, 5 },    // dam
            .f_inv_efuse     = { false, 0, 5 },   // f_inv
            .r_inv_efuse     = { false, 0, 5 },   // r_inv
            .r_rad_fan_efuse = { false, 200, 5 }, // r_rad_fan
            .l_rad_fan_efuse = { false, 200, 5 }, // l_rad_fan
            .rr_pump_efuse   = { false, 200, 5 }, // rr_pump
            .rl_pump_efuse   = { false, 200, 5 }, // rl_pump
        };
        app::powerManager::updateConfig(power_manager_state);
        can_tx::VC_State_set(VCState::VC_FAULT_STATE);
        LOG_INFO("entering fault state!");
    }

    static void runOnTick100Hz()
    {
        if (!can_alerts::AnyBoardHasFault()) // all boards do not have faults
        {
            StateMachine::set_next_state(&init_state);
        }
    }

    static void runOnExit()
    {
        LOG_INFO("exiting fault state!");
    }

} // namespace faultState

State fault_state = { .name              = "Fault State",
                      .run_on_entry      = faultState::runOnEntry,
                      .run_on_tick_1Hz   = nullptr,
                      .run_on_tick_100Hz = faultState::runOnTick100Hz,
                      .run_on_exit       = faultState::runOnExit };

} // namespace app::states
