#include "torque_vectoring/estimation/dynamics_estimation.hpp"
#include "torque_vectoring/estimation/tire_model.hpp"
#include "torque_vectoring/estimation/vehicle_state_estimator.hpp"
#include "torque_vectoring/controllers/yaw_rate_control/controllers_dyrc.hpp"
#include "torque_vectoring/controllers/torque_allocator/torque_allocator.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"
#include "torque_vectoring/shared_datatypes/low_speed_blend.hpp"

#include <cmath>

namespace app::tv
{
namespace
{
    using namespace shared_datatypes::datatypes;
    using namespace shared_datatypes::vd_constants;

    //------------------------------------- ESTIMATION MODULES ----------------------------------//
    wheel_set tire_models{
        .fl = estimation::TireModel(estimation::TireModel::WheelSide::Left, estimation::TireModel::WheelAxle::Front),
        .fr = estimation::TireModel(estimation::TireModel::WheelSide::Right, estimation::TireModel::WheelAxle::Front),
        .rl = estimation::TireModel(estimation::TireModel::WheelSide::Left, estimation::TireModel::WheelAxle::Rear),
        .rr = estimation::TireModel(estimation::TireModel::WheelSide::Right, estimation::TireModel::WheelAxle::Rear),
    };

    //------------------------------------- STATE VARIABLES -------------------------------------//

    // Build TireModel::StateInputs from VehicleState + per-wheel data
    // [[nodiscard]] estimation::TireModel::StateInputs
    //     buildTireInputs(const VehicleState &state, const float omega, const float fz)
    // {
    //     return {
    //         .wheel_angular_velocity_radps = omega,
    //         .vehicle_velocity_x_mps       = state.v_x_mps,
    //         .vehicle_velocity_y_mps       = state.v_y_mps,
    //         .yaw_rate_radps               = state.yaw_rate_radps,
    //         .steering_angle_rad           = state.steer_ang_rad,
    //         .normal_load_N                = fz,
    //     };
    // }

    // These wheel-set caches carry the previous update's estimated tire state/forces into the next cycle.
    // The state estimator and optimizer both consume them as warm starts / feedback, so they remain module-local.
    // wheel_set<float> acc_f_x{};
    // wheel_set<float> acc_f_y{};
    // wheel_set<float> acc_f_z{};
    // wheel_set<float> acc_slip_ratio{};
    // wheel_set<float> acc_slip_angle{};
    // wheel_set<float> slip_ratio_opt{};
    // Copy the current tire estimates into the module-local wheel-set caches so the rest of the
    // pipeline can use a consistent per-wheel state snapshot for this update tick.
    // void unpackTireOutputs(
    //     const estimation::TireModel::Outputs &fl,
    //     const estimation::TireModel::Outputs &fr,
    //     const estimation::TireModel::Outputs &rl,
    //     const estimation::TireModel::Outputs &rr)
    // {
    //     acc_f_x = { .fl = fl.longitudinal_force_N,
    //                 .fr = fr.longitudinal_force_N,
    //                 .rl = rl.longitudinal_force_N,
    //                 .rr = rr.longitudinal_force_N };
    //     acc_f_y = {
    //         .fl = fl.lateral_force_N, .fr = fr.lateral_force_N, .rl = rl.lateral_force_N, .rr = rr.lateral_force_N
    //     };
    //     acc_slip_ratio = { .fl = fl.slip_ratio, .fr = fr.slip_ratio, .rl = rl.slip_ratio, .rr = rr.slip_ratio };
    //     acc_slip_angle = {
    //         .fl = fl.slip_angle_rad, .fr = fr.slip_angle_rad, .rl = rl.slip_angle_rad, .rr = rr.slip_angle_rad
    //     };
    // }
} // namespace

//------------------------------------- TORQUE VECTORING UPDATE CYCLE -------------------------//
ControlOutput update(const estimation::Measurements &measurement)
{
    //------------------------------------- STATE ESTIMATION  --------------------------------//
    // Fuse the measured chassis state with the previous tire-force estimate to obtain the
    // filtered vehicle state used consistently by all downstream control blocks this tick.
    const VehicleState state = estimation::VehicleStateEstimator::estimate(measurement);

    // Normal forces from longitudinal/lateral load transfer + downforce
    // acc_f_z = vehicle_dynamics_estimator.est_Fz_N(
    //     estimated_state.a_x_mps2, estimated_state.a_y_mps2, estimated_state.v_x_mps);
    // Tire model estimation from current sensor data using the filtered vehicle state.
    // unpackTireOutputs(
    //     tire_models.fl.estimate(buildTireInputs(estimated_state, fl_omega, acc_f_z.fl)),
    //     tire_models.fr.estimate(buildTireInputs(estimated_state, fr_omega, acc_f_z.fr)),
    //     tire_models.rl.estimate(buildTireInputs(estimated_state, rl_omega, acc_f_z.rl)),
    //     tire_models.rr.estimate(buildTireInputs(estimated_state, rr_omega, acc_f_z.rr)));

    //------------------------------------- HIGH LEVEL CONTROLLER ----------------------------//

    const float ax_mps2_setpoint = MAX_AX * measurement.apps;
    // Direct yaw rate control: corrective yaw moment
    const float dotomega_radps2_setpoint =
        controllers::dyrc::computeYawMoment(state.yaw_rate_radps, state.steer_ang_rad, state.v_x_mps);

    //------------------------------------- LOW LEVEL CONTROLLER -----------------------------//

    // Compute the low-speed blend once at the orchestration layer and pass it down explicitly.
    // This keeps the low-speed force-availability heuristic visible in one place instead of
    // recomputing it independently inside the optimizer.
    const float vehicle_speed_mps = std::hypot(estimated_state.v_x_mps, estimated_state.v_y_mps);
    const float low_speed_blend   = shared_datatypes::velocityBlend(vehicle_speed_mps);

    // Gauss-Newton optimizer: invert the combined-slip tire model to find the slip ratios whose
    // predicted forces best match the desired per-wheel force split and desired yaw moment.
    // ReSharper disable once CppUseStructuredBinding
    const wheel_set<float> kappa_opt =
        controllers::TorqueAllocator::optimize(state, ax_mps2_setpoint, dotomega_radps2_setpoint);

    //------------------------------------- POWER LIMITER -----------------------------------//
    // TODO: slip_ratio_opt -> slipRatioToWheelAngularVelocity() -> power limiter -> torque request

    return {
        .fl_kappa = kappa_opt.fl,
        .fr_kappa = kappa_opt.fr,
        .rl_kappa = kappa_opt.rl,
        .rr_kappa = kappa_opt.rr,
    };
}

ControlOutputAutonomous
    update_autonomous(const estimation::Measurements &measurement, const float ax, const float omega_dot)
{
    // TODO inshallah one day
    return {};
}
} // namespace app::tv
