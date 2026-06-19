#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_inverter.hpp"
#include "app_powerManager.hpp"
#include "app_startSwitch.hpp"
#include "app_states.hpp"
#include "app_bspdwarning.hpp"

#include "torque_vectoring/datatypes/torque_limits.hpp"

#include "io_log.hpp"

using namespace app::can_utils;
using namespace app::inverter;
using namespace app::powerManager;
using namespace app::tv::datatypes::torque_limits;

static constexpr Efuses<EfuseConfig> power_manager_state = {
    .front_efuse     = { true, 0, 5 },   // front
    .rsm_efuse       = { true, 0, 5 },   // rsm
    .bms_efuse       = { true, 0, 5 },   // bms
    .dam_efuse       = { true, 0, 5 },   // dam
    .f_inv_efuse     = { true, 0, 5 },   // f_inv
    .r_inv_efuse     = { true, 0, 5 },   // r_inv
    .r_rad_fan_efuse = { true, 200, 5 }, // r_rad_fan
    .l_rad_fan_efuse = { true, 200, 5 }, // l_rad_fan
    .rr_pump_efuse   = { true, 200, 5 }, // rr_pump
    .rl_pump_efuse   = { true, 200, 5 }, // rl_pump
};

namespace app::states
{
static void driveStateRunOnEntry()
{
    LOG_INFO("entering drive state!");

    // enable inverters
    can_tx::VC_State_set(VCState::VC_DRIVE_STATE);
    updateConfig(power_manager_state);

    // Ensure inverters are enabled
    inverter_enable_toggle(true, true, true, true);

    set_torque_limit_negative(MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm);
    set_torque_limit_positive(
        MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm);
    send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
}

static void driveStateRunOnTick100Hz()
{
    if (can_rx::BMS_State_get() == BmsState::BMS_INIT_STATE)
    {
        StateMachine::set_next_state(&init_state);
        return;
    }
    if (startSwitch::hasRisingEdge())
    {
        StateMachine::set_next_state(&hv_state);
        return;
    }

    // TODO check inverter preconditions, return to hv init if not fulfilled

    const float apps = can_rx::FSM_PappsMappedPedalPercentage_get();

if ((can_alerts::AnyBoardHasWarning() && (app::can_rx::CRIT_LaunchControlSwitch_get() != SwitchState::ON)) || app::bspdWarning::checkSoftwareBspd(apps))    {
        send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
        return;
    }
    // TODO: add driving algorithm handling here
    const float pedal_torque_request = apps * MAX_TORQUE_REQUEST_Nm / 100.0f;

    // inverters expect smoother signal
    // just for spinning wheels
    const float torque_fl = app::can_rx::Debug_TorqueRequest_FL_get();
    const float torque_fr = app::can_rx::Debug_TorqueRequest_FR_get();
    const float torque_rl = app::can_rx::Debug_TorqueRequest_RL_get();
    const float torque_rr = app::can_rx::Debug_TorqueRequest_RR_get();
    if (torque_fl < 0.1f && torque_fr < 0.1f && torque_rl < 0.1f && torque_rr < 0.1f)
    {
        send_torque(pedal_torque_request, pedal_torque_request, pedal_torque_request, pedal_torque_request);
    }
    else
    {
        send_torque(torque_fl, torque_fr, torque_rl, torque_rr);
    }
}

static void driveStateRunOnExit()
{
    // disable inverters
    LOG_INFO("exiting drive state!");
    set_torque_limit_negative(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
    set_torque_limit_positive(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
}

State drive_state = { .name              = "DRIVE",
                      .run_on_entry      = driveStateRunOnEntry,
                      .run_on_tick_1Hz   = nullptr,
                      .run_on_tick_100Hz = driveStateRunOnTick100Hz,
                      .run_on_exit       = driveStateRunOnExit };
} // namespace app::states
