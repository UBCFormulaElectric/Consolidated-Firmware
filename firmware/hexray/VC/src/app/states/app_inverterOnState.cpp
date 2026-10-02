#include "app_stateMachine.hpp"
#include "app_canTx.hpp"
#include "app_states.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "io_log.hpp"
#include "io_pcm.hpp"

#include "app_powerManager.hpp"

using namespace app::can_utils;
namespace app::states
{
namespace inverterOnState
{
    static void runOnEntry()
    {
        LOG_INFO("entering inverter on state!");
        static constexpr powerManager::Efuses<powerManager::EfuseConfig> power_manager_state = {
            .front_efuse     = { true, 0, 5 },    // front
            .rsm_efuse       = { true, 0, 5 },    // rsm
            .bms_efuse       = { true, 0, 5 },    // bms
            .dam_efuse       = { true, 0, 5 },    // dam
            .f_inv_efuse     = { true, 200, 5 },  // f_inv
            .r_inv_efuse     = { true, 200, 5 },  // r_inv
            .r_rad_fan_efuse = { false, 200, 5 }, // r_rad_fan
            .l_rad_fan_efuse = { false, 200, 5 }, // l_rad_fan
            .rr_pump_efuse   = { false, 200, 5 }, // rr_pump
            .rl_pump_efuse   = { false, 200, 5 }, // rl_pump
        };
        app::powerManager::updateConfig(power_manager_state);

        can_tx::VC_State_set(VCState::VC_INVERTER_ON_STATE);
        io::pcm::set(false);
    }

    static void runOnTick100Hz()
    {
        if (const ContactorState air_minus_closed = can_rx::BMS_IrNegative_get();
            air_minus_closed == ContactorState::CONTACTOR_STATE_OPEN)
        {
            StateMachine::set_next_state(&init_state);
        }
        const bool inverters_bsystemReady = can_rx::INVFL_bSystemReady_get() && can_rx::INVFR_bSystemReady_get() &&
                                            can_rx::INVRL_bSystemReady_get() && can_rx::INVRR_bSystemReady_get();

        if (inverters_bsystemReady)
        {
            StateMachine::set_next_state(&bmsOn_state);
        }

        // TODO inverter fault handling
    }

    static void runOnExit()
    {
        LOG_INFO("exiting inverter on state!");
    }
} // namespace inverterOnState

State inverterOn_state = { .name              = "INVERTER ON",
                           .run_on_entry      = inverterOnState::runOnEntry,
                           .run_on_tick_1Hz   = nullptr,
                           .run_on_tick_100Hz = inverterOnState::runOnTick100Hz,
                           .run_on_exit       = inverterOnState::runOnExit };
} // namespace app::states
