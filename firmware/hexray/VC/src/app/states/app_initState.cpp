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
#include "app_pumpControl.hpp"

using namespace app::can_utils;
using namespace app::tv::datatypes;

namespace app::states
{

namespace initState
{
    static void reset_inverter_rx()
    {
        app::can_rx::INVFL_bSystemReady_update(false);
        app::can_rx::INVFR_bSystemReady_update(false);
        app::can_rx::INVRL_bSystemReady_update(false);
        app::can_rx::INVRR_bSystemReady_update(false);
        app::can_rx::INVFL_bQuitDcOn_update(false);
        app::can_rx::INVFR_bQuitDcOn_update(false);
        app::can_rx::INVRL_bQuitDcOn_update(false);
        app::can_rx::INVRR_bQuitDcOn_update(false);
        app::can_rx::INVFL_bQuitInverterOn_update(false);
        app::can_rx::INVFR_bQuitInverterOn_update(false);
        app::can_rx::INVRL_bQuitInverterOn_update(false);
        app::can_rx::INVRR_bQuitInverterOn_update(false);
        app::can_rx::INVFL_bError_update(false);
        app::can_rx::INVFR_bError_update(false);
        app::can_rx::INVRL_bError_update(false);
        app::can_rx::INVRR_bError_update(false);
        app::can_rx::INVFL_ErrorInfo_update(0u);
        app::can_rx::INVFR_ErrorInfo_update(0u);
        app::can_rx::INVRL_ErrorInfo_update(0u);
        app::can_rx::INVRR_ErrorInfo_update(0u);
    }

    static void reset_inverter_tx()
    {
        app::can_tx::VC_INVFLbDcOn_set(false);
        app::can_tx::VC_INVFRbDcOn_set(false);
        app::can_tx::VC_INVRLbDcOn_set(false);
        app::can_tx::VC_INVRRbDcOn_set(false);
        app::can_tx::VC_INVFLbEnable_set(false);
        app::can_tx::VC_INVFRbEnable_set(false);
        app::can_tx::VC_INVRLbEnable_set(false);
        app::can_tx::VC_INVRRbEnable_set(false);
        app::can_tx::VC_INVFLbInverterOn_set(false);
        app::can_tx::VC_INVFRbInverterOn_set(false);
        app::can_tx::VC_INVRLbInverterOn_set(false);
        app::can_tx::VC_INVRRbInverterOn_set(false);
        app::can_tx::VC_INVFLbErrorReset_set(false);
        app::can_tx::VC_INVFRbErrorReset_set(false);
        app::can_tx::VC_INVRLbErrorReset_set(false);
        app::can_tx::VC_INVRRbErrorReset_set(false);
    }

    static constexpr app::powerManager::PowerManagerConfig power_manager_state = {
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

        app::can_rx::clear_board_rx_table(app::can_utils::CanNode::VC_NODE);

        app::powerManager::updateConfig(power_manager_state);
        app::can_tx::VC_State_set(VCState::VC_INIT_STATE);

        app::can_alerts::infos::InverterRetry_set(false);
        reset_inverter_rx();
        reset_inverter_tx();

        app::can_tx::VC_INVFLTorqueSetpoint_set(torque_limits::NO_TORQUE_Nm);
        app::can_tx::VC_INVFRTorqueSetpoint_set(torque_limits::NO_TORQUE_Nm);
        app::can_tx::VC_INVRLTorqueSetpoint_set(torque_limits::NO_TORQUE_Nm);
        app::can_tx::VC_INVRRTorqueSetpoint_set(torque_limits::NO_TORQUE_Nm);

        app::can_tx::VC_INVFLTorqueLimitPositive_set(torque_limits::NO_TORQUE_Nm);
        app::can_tx::VC_INVFRTorqueLimitPositive_set(torque_limits::NO_TORQUE_Nm);
        app::can_tx::VC_INVRLTorqueLimitPositive_set(torque_limits::NO_TORQUE_Nm);
        app::can_tx::VC_INVRRTorqueLimitPositive_set(torque_limits::NO_TORQUE_Nm);

        app::can_tx::VC_INVFLTorqueLimitNegative_set(torque_limits::NO_TORQUE_Nm);
        app::can_tx::VC_INVFRTorqueLimitNegative_set(torque_limits::NO_TORQUE_Nm);
        app::can_tx::VC_INVRLTorqueLimitNegative_set(torque_limits::NO_TORQUE_Nm);
        app::can_tx::VC_INVRRTorqueLimitNegative_set(torque_limits::NO_TORQUE_Nm);
        app::pumpControl::restart();
    }

    static void runOnTick100Hz()
    {
        app::pumpControl::MonitorPumps();
        if (const ContactorState air_minus_closed = app::can_rx::BMS_IrNegative_get();
            air_minus_closed == ContactorState::CONTACTOR_STATE_CLOSED)
        {
            app::StateMachine::set_next_state(&inverterOn_state);
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
