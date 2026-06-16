#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_powerManager.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_canAlerts.hpp"
#include "app_pumpControl.hpp"
#include "torque_vectoring/datatypes/torque_limits.hpp"
#include "io_log.hpp"
#include "io_pcm.hpp"

using namespace app::can_utils;
using namespace app::tv::datatypes;

namespace app::states
{

namespace initState
{
    static constexpr powerManager::Efuses<powerManager::EfuseConfig> power_manager_state = {
        .front_efuse     = { true, 0, 5 },
        .rsm_efuse       = { true, 0, 5 },
        .bms_efuse       = { true, 0, 5 },
        .dam_efuse       = { true, 0, 5 },
        .f_inv_efuse     = { false, 0, 5 },
        .r_inv_efuse     = { false, 0, 5 },
        .r_rad_fan_efuse = { false, 200, 5 },
        .l_rad_fan_efuse = { false, 200, 5 },
        .rr_pump_efuse   = { false, 200, 5 },
        .rl_pump_efuse   = { false, 200, 5 },
    };

    static void runOnEntry()
    {
        LOG_INFO("entering init state!");

        io::pcm::set(false);

        can_rx::clear_board_rx_table(CanNode::INVFL_NODE);
        can_rx::clear_board_rx_table(CanNode::INVFR_NODE);
        can_rx::clear_board_rx_table(CanNode::INVRL_NODE);
        can_rx::clear_board_rx_table(CanNode::INVRR_NODE);

        app::powerManager::updateConfig(power_manager_state);
        can_tx::VC_State_set(VCState::VC_INIT_STATE);

        can_alerts::infos::InverterRetry_set(false);

        can_tx::VC_INVFLTorqueSetpoint_set(torque_limits::NO_TORQUE_Nm);
        can_tx::VC_INVFRTorqueSetpoint_set(torque_limits::NO_TORQUE_Nm);
        can_tx::VC_INVRLTorqueSetpoint_set(torque_limits::NO_TORQUE_Nm);
        can_tx::VC_INVRRTorqueSetpoint_set(torque_limits::NO_TORQUE_Nm);

        can_tx::VC_INVFLTorqueLimitPositive_set(torque_limits::NO_TORQUE_Nm);
        can_tx::VC_INVFRTorqueLimitPositive_set(torque_limits::NO_TORQUE_Nm);
        can_tx::VC_INVRLTorqueLimitPositive_set(torque_limits::NO_TORQUE_Nm);
        can_tx::VC_INVRRTorqueLimitPositive_set(torque_limits::NO_TORQUE_Nm);

        can_tx::VC_INVFLTorqueLimitNegative_set(torque_limits::NO_TORQUE_Nm);
        can_tx::VC_INVFRTorqueLimitNegative_set(torque_limits::NO_TORQUE_Nm);
        can_tx::VC_INVRLTorqueLimitNegative_set(torque_limits::NO_TORQUE_Nm);
        can_tx::VC_INVRRTorqueLimitNegative_set(torque_limits::NO_TORQUE_Nm);
        pumpControl::restart();
    }

    static void runOnTick100Hz()
    {
        if (const ContactorState air_minus_closed = can_rx::BMS_IrNegative_get();
            air_minus_closed == ContactorState::CONTACTOR_STATE_CLOSED)
        {
            StateMachine::set_next_state(&inverterOn_state);
        }
    }

    static void runOnExit()
    {
        LOG_INFO("exiting init state!");
    }
} // namespace initState

State init_state = { .name              = "INIT",
                     .run_on_entry      = initState::runOnEntry,
                     .run_on_tick_1Hz   = nullptr,
                     .run_on_tick_100Hz = initState::runOnTick100Hz,
                     .run_on_exit       = initState::runOnExit };
} // namespace app::states
