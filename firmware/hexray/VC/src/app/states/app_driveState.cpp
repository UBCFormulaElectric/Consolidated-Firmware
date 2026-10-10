#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_inverter.hpp"
#include "app_powerManager.hpp"
#include "app_startSwitch.hpp"
#include "app_states.hpp"
#include "app_bspdwarning.hpp"
#include "app_imu.hpp"

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/torque_limits.hpp"
#include "torque_vectoring/torque_vectoring.hpp"
#include "torque_vectoring/estimation/vehicle_state_estimator.hpp"
#include "torque_vectoring/tv_debug.h"
#include "util_units.hpp"

#include "io_log.hpp"
#include "io_pcm.hpp"
#include "io_imus.hpp"

using namespace app::can_utils;
using namespace app::inverter;
using namespace app::powerManager;
using namespace app::tv::datatypes::torque_limits;

static constexpr Efuses<EfuseConfig> power_manager_state = {
    .front_efuse     = { true, 0, 5 },    // front
    .rsm_efuse       = { true, 0, 5 },    // rsm
    .bms_efuse       = { true, 0, 5 },    // bms
    .dam_efuse       = { true, 0, 5 },    // dam
    .f_inv_efuse     = { true, 200, 5 },  // f_inv
    .r_inv_efuse     = { true, 200, 5 },  // r_inv
    .r_rad_fan_efuse = { true, 200, 5 },  // r_rad_fan
    .l_rad_fan_efuse = { true, 200, 5 },  // l_rad_fan
    .rr_pump_efuse   = { true, 500, 10 }, // rr_pump
    .rl_pump_efuse   = { true, 500, 10 }, // rl_pump
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

    io::pcm::set(true);
    set_torque_limit_negative(MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm);
    set_torque_limit_positive(
        MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm);
    send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
}

static void driveStateRunOnTick100Hz()
{
    const bool bms_drive_dropped     = can_rx::BMS_State_get() != BmsState::BMS_DRIVE_STATE;
    const bool drive_allowed_dropped = !inverter::drive_allowed();
    if (bms_drive_dropped)
    {
        StateMachine::set_next_state(&init_state);
        return;
    }
    else if (drive_allowed_dropped)
    {
        StateMachine::set_next_state(&hvInit_state);
        return;
    }
    else
    {
        // Do nothing
    }

    if (startSwitch::hasRisingEdge())
    {
        StateMachine::set_next_state(&hv_state);
        return;
    }

    // TODO check inverter preconditions, return to hv init if not fulfilled

    const auto apps_percentage = can_rx::FSM_PappsMappedPedalPercentage_get();
    app::bspdWarning::checkSoftwareBspd(apps_percentage);

    if (can_alerts::AnyBoardHasWarning() and app::can_rx::CRIT_LaunchControlSwitch_get() != SwitchState::ON)
    {
        send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
        return;
    }

    // state estimator
    // The estimator works in SI units: the IMU driver gives g and deg/s, the inverters give motor rpm and the FSM
    // gives the steering wheel angle in deg, so convert here.
    static std::size_t tick = 0;

    if (tick++ % 5 == 0)
    {
        using app::tv::shared_datatypes::vd_constants::GRAVITY;

        const app::tv::estimation::Measurements meas = {
            .ax_mps2        = app::imu::getAccelX().value_or(0.0f) * GRAVITY,
            .ay_mps2        = app::imu::getAccelY().value_or(0.0f) * GRAVITY,
            .yaw_rate_radps = DEG_TO_RAD(app::imu::getGyroZ().value_or(0.0f)),
            .omegas   = {
                .fl = MOTOR_RPM_TO_WHEEL_RADPS(static_cast<float>(app::can_rx::INVFL_ActualVelocity_get())),
                .fr = MOTOR_RPM_TO_WHEEL_RADPS(static_cast<float>(app::can_rx::INVFR_ActualVelocity_get())),
                .rl = MOTOR_RPM_TO_WHEEL_RADPS(static_cast<float>(app::can_rx::INVRL_ActualVelocity_get())),
                .rr = MOTOR_RPM_TO_WHEEL_RADPS(static_cast<float>(app::can_rx::INVRR_ActualVelocity_get())),
            },
            .delta        = DEG_TO_RAD(app::can_rx::FSM_SteeringAngle_get()),
            .gps_vx_mps   = app::sbgEllipse::bodyVelX(),
            .gps_vy_mps   = app::sbgEllipse::bodyVelY(),
            .sbg_ekf_mode = app::sbgEllipse::getEkfSolutionMode(),
        };

        const app::tv::shared_datatypes::VehicleState<float> vehicle_state =
            app::tv::estimation::VehicleStateEstimator::estimate(meas);

        const veh_ekf_info &ekf = tv_debug_data.ekf_info;
        can_tx::VC_EkfEstVx_set(vehicle_state.v_x_mps);
        can_tx::VC_EkfEstVy_set(vehicle_state.v_y_mps);
        can_tx::VC_EkfPredVx_set(static_cast<float>(ekf.pred_v_x));
        can_tx::VC_EkfPredVy_set(static_cast<float>(ekf.pred_v_y));
        can_tx::VC_EkfPredMotorRpmFL_set(WHEEL_RADPS_TO_MOTOR_RPM(static_cast<float>(ekf.pred_omegas[0])));
        can_tx::VC_EkfPredMotorRpmFR_set(WHEEL_RADPS_TO_MOTOR_RPM(static_cast<float>(ekf.pred_omegas[1])));
        can_tx::VC_EkfPredMotorRpmRL_set(WHEEL_RADPS_TO_MOTOR_RPM(static_cast<float>(ekf.pred_omegas[2])));
        can_tx::VC_EkfPredMotorRpmRR_set(WHEEL_RADPS_TO_MOTOR_RPM(static_cast<float>(ekf.pred_omegas[3])));
        can_tx::VC_EkfCovVxVx_set(static_cast<float>(ekf.covariance[0]));
        can_tx::VC_EkfCovVxVy_set(static_cast<float>(ekf.covariance[1])); // P is symmetric: covariance[2] == [1]
        can_tx::VC_EkfCovVyVy_set(static_cast<float>(ekf.covariance[3]));
    }

    // TODO: add driving algorithm handling here
    const float pedal_torque_request = apps_percentage * MAX_TORQUE_REQUEST_Nm / 100.0f;

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
