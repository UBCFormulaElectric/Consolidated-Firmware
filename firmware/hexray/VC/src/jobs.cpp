#include "jobs.hpp"

#include "app/app_inverter.hpp"
#include "app/app_powerManager.hpp"
#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_lowVoltageBattery.hpp"
#include "app_powerMonitoring.hpp"
#include "app_sbgEllipse.hpp"
#include "app/states/app_states.hpp"
#include "app_stateMachine.hpp"
#include "app_timer.hpp"
#include "app_commitInfo.hpp"
#include "app_imu.hpp"
#include "app_pumpControl.hpp"
#include "app_vcShdnLoop.hpp"
#include "app_shdnLast.hpp"

#include "io_canMsg.hpp"
#include "io_canTx.hpp"
#include "io_canQueues.hpp"
#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_canReroute.hpp"
#include "io_sbgEllipse.hpp"
#include "io_pcm.hpp"

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/torque_limits.hpp"
#include "torque_vectoring/torque_vectoring.hpp"
#include "torque_vectoring/estimation/vehicle_state_estimator.hpp"
#include "torque_vectoring/tv_debug.h"

#include <util_errorCodes.hpp>
#include "util_units.hpp"

static void fdcan_tx(const JsonCanMsg &tx_msg)
{
    const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    LOG_IF_ERR(fdcan_tx_queue.push(msg));
}
static void invcan_tx(const JsonCanMsg &tx_msg)
{
    const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    LOG_IF_ERR(invcan_tx_queue.push(msg));
}

static constexpr uint32_t AIR_MINUS_OPEN_DEBOUNCE_MS = 100U;
static app::Timer         air_minus_open_debounce_timer{ AIR_MINUS_OPEN_DEBOUNCE_MS };

void jobs_init()
{
    fdcan_tx_queue.init();
    invcan_tx_queue.init();
    can_rx_queue.init();

    io::can_tx::init(fdcan_tx, invcan_tx);
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    io::can_tx::enableMode_InvCAN(app::can_utils::InvCANMode::INVCAN_MODE_DEFAULT, true);

    io::can_reroute::init(fdcan_tx, invcan_tx);

    app::can_tx::VC_Hash_set(GIT_COMMIT_HASH);
    app::can_tx::VC_Clean_set(GIT_COMMIT_CLEAN);
    app::can_tx::VC_Heartbeat_set(true);

    io::can_tx::VC_Bootup_sendAperiodic();

    app::pumpControl::restart();

    app::StateMachine::init(&app::states::init_state);
}
void jobs_initImu()
{
    app::imu::init();
}
void jobs_run1Hz_tick() {}
void jobs_run100Hz_tick()
{
    if (app::can_alerts::AnyBoardHasFault())
    {
        app::StateMachine::set_next_state(&app::states::fault_state);
    }

    app::StateMachine::tick100Hz();

    if (app::StateMachine::get_current_state() == &app::states::inverterOn_state ||
        app::StateMachine::get_current_state() == &app::states::hvInit_state ||
        app::StateMachine::get_current_state() == &app::states::hv_state ||
        app::StateMachine::get_current_state() == &app::states::drive_state ||
        app::StateMachine::get_current_state() == &app::states::inverter_fault_handling_state)
    {
        app::inverter::FaultCheck();
    }

    // pcm state broadcasting
    app::can_tx::VC_PcmEnable_set(io::pcm::enabled());

    // efuse set
    app::powerManager::efuseProtocolTick_100Hz();
    app::powerManager::broadcastRetryCounts();

    // heartbeats
    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();

    // shdn
    app::shdnLoop::broadcast();
    app::shdnLast::broadcast();

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
            .apps         = app::can_rx::FSM_PappsMappedPedalPercentage_get(),
            .brake        = static_cast<float>(app::can_rx::FSM_FrontBrakePressure_get()),
            .gps_vx_mps   = app::sbgEllipse::bodyVelX(),
            .gps_vy_mps   = app::sbgEllipse::bodyVelY(),
            .discharge_power_limit = static_cast<float>(app::can_rx::BMS_ChargePowerLimit_get()),
            .charge_power_limit = static_cast<float>(app::can_rx::BMS_DischargePowerLimit_get()),
            .sbg_ekf_mode = app::sbgEllipse::getEkfSolutionMode(),
        };

        const app::tv::shared_datatypes::VehicleState<float> vehicle_state =
            app::tv::estimation::VehicleStateEstimator::estimate(meas);

        const veh_ekf_info &ekf = tv_debug_data.ekf_info;
        app::can_tx::VC_EkfEstVx_set(vehicle_state.v_x_mps);
        app::can_tx::VC_EkfEstVy_set(vehicle_state.v_y_mps);
        app::can_tx::VC_EkfPredVx_set(static_cast<float>(ekf.pred_v_x));
        app::can_tx::VC_EkfPredVy_set(static_cast<float>(ekf.pred_v_y));
        app::can_tx::VC_EkfPredMotorRpmFL_set(WHEEL_RADPS_TO_MOTOR_RPM(static_cast<float>(ekf.pred_omegas[0])));
        app::can_tx::VC_EkfPredMotorRpmFR_set(WHEEL_RADPS_TO_MOTOR_RPM(static_cast<float>(ekf.pred_omegas[1])));
        app::can_tx::VC_EkfPredMotorRpmRL_set(WHEEL_RADPS_TO_MOTOR_RPM(static_cast<float>(ekf.pred_omegas[2])));
        app::can_tx::VC_EkfPredMotorRpmRR_set(WHEEL_RADPS_TO_MOTOR_RPM(static_cast<float>(ekf.pred_omegas[3])));
        app::can_tx::VC_EkfCovVxVx_set(static_cast<float>(ekf.covariance[0]));
        app::can_tx::VC_EkfCovVxVy_set(static_cast<float>(ekf.covariance[1])); // P is symmetric: covariance[2] == [1]
        app::can_tx::VC_EkfCovVyVy_set(static_cast<float>(ekf.covariance[3]));
        app::can_tx::VC_EkfZeroVxActive_set(ekf.zero_vx_active > 0.5);
        app::can_tx::VC_EkfZeroVyActive_set(ekf.zero_vy_active > 0.5);
    }

    // pumps
    app::pumpControl::MonitorPumps();

    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
void jobs_runImu_tick()
{
    app::imu::broadcast();
}
void jobs_runBatteryMonitoring_tick()
{
    LOG_IF_ERR(app::batteryMonitoring::update());
}
void jobs_runPowerMonitoring_tick()
{
    LOG_IF_ERR(app::powerMonitoring::update());
}
void jobs_runSbgEllipse_tick()
{
    io::sbgEllipse::handleLogs();
    app::sbgEllipse::broadcast();
}