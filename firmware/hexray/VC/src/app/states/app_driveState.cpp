#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_inverter.hpp"
#include "app_powerManager.hpp"
#include "app_pumpControl.hpp"
#include "app_states.hpp"

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

static volatile float apps = 0.0f;

namespace app::states
{
// static bool driveStatePassPreCheck()
// {
//     // check inverter warnings
//     app::inverter::FaultCheck();

//     // check board warnings
//     // if (app::can_alerts::AnyBoardHasWarning())
//     //     return false;

//     // check possibly other faults
//     apps = app::can_rx::FSM_PappsMappedPedalPercentage_get();

//     if (app::bspdWarning::checkSoftwareBspd(apps))
//         return false;

//     return true;
// }

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
    }
    else
    {
        // efuseProtocolTick_100Hz();
        // if (!driveStatePassPreCheck())
        // {
        //     send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
        //     // TODO: set speed requests to 0 as well
        //     return;
        // }

        // TODO: add driving algorithm handling here
        // just for spinning wheels
        apps               = can_rx::FSM_PappsMappedPedalPercentage_get();
        const float torque = apps * MAX_TORQUE_REQUEST_Nm / 100.0f;
        send_torque(torque, torque, torque, torque);
    }
}

static void driveStateRunOnExit()
{
    // disable inverters
    LOG_INFO("exiting drive state!");
    inverter_enable_toggle(false, false, false, false);
    set_torque_limit_negative(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
    set_torque_limit_positive(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
}

State drive_state = { .name              = "DRIVE",
                      .run_on_entry      = driveStateRunOnEntry,
                      .run_on_tick_1Hz   = nullptr,
                      .run_on_tick_100Hz = driveStateRunOnTick100Hz,
                      .run_on_exit       = driveStateRunOnExit };
} // namespace app::states
