#include "torque_vectoring/estimation/dynamics_estimation.hpp"
#include "torque_vectoring/estimation/tire_model.hpp"
#include "torque_vectoring/estimation/vehicle_state_estimator.hpp"
#include "torque_vectoring/controllers/yaw_rate_control/controllers_dyrc.hpp"
#include "torque_vectoring/controllers/torque_allocator/torque_allocator.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"

#include <cmath>

namespace app::tv
{
namespace
{
using namespace datatypes::datatypes;
using namespace datatypes::vd_constants;

//------------------------------------- ESTIMATION MODULES ----------------------------------//

static estimation::vehicleDynamics vehicle_dynamics_estimator{};
static estimation::VehicleStateEstimator vehicle_state_estimator{};

static estimation::TireModel fl_tire_model{
    estimation::TireModel::TirePressure::PSI_12,
    estimation::TireModel::WheelSide::Left,
    estimation::TireModel::WheelAxle::Front};

static estimation::TireModel fr_tire_model{
    estimation::TireModel::TirePressure::PSI_12,
    estimation::TireModel::WheelSide::Right,
    estimation::TireModel::WheelAxle::Front};

static estimation::TireModel rl_tire_model{
    estimation::TireModel::TirePressure::PSI_12,
    estimation::TireModel::WheelSide::Left,
    estimation::TireModel::WheelAxle::Rear};

static estimation::TireModel rr_tire_model{
    estimation::TireModel::TirePressure::PSI_12,
    estimation::TireModel::WheelSide::Right,
    estimation::TireModel::WheelAxle::Rear};

//------------------------------------- CONTROLLERS -----------------------------------------//

static controllers::allocator::TorqueAllocator torque_allocator{};

//------------------------------------- STATE VARIABLES -------------------------------------//

static wheel_set acc_f_x{};
static wheel_set acc_f_y{};
static wheel_set acc_f_z{};
static wheel_set acc_slip_ratio{};
static wheel_set acc_slip_angle{};
static wheel_set slip_ratio_opt{};
[[maybe_unused]] static float est_yaw_moment_nm{};

// Build TireModel::StateInputs from VehicleState + per-wheel data
[[nodiscard]] estimation::TireModel::StateInputs buildTireInputs(
    const VehicleState& state, const float omega, const float fz)
{
    return {
        .wheel_angular_velocity_radps = omega,
        .vehicle_velocity_x_mps       = state.v_x_mps,
        .vehicle_velocity_y_mps       = state.v_y_mps,
        .yaw_rate_radps               = state.yaw_rate_radps,
        .steering_angle_rad           = state.steer_ang_rad,
        .normal_load_N                = fz,
    };
}

// Unpack four tire model outputs into the module-level wheel_set variables
void unpackTireOutputs(
    const estimation::TireModel::Outputs& fl,
    const estimation::TireModel::Outputs& fr,
    const estimation::TireModel::Outputs& rl,
    const estimation::TireModel::Outputs& rr)
{
    acc_f_x = { .fl = fl.longitudinal_force_N, .fr = fr.longitudinal_force_N,
                .rl = rl.longitudinal_force_N, .rr = rr.longitudinal_force_N };
    acc_f_y = { .fl = fl.lateral_force_N, .fr = fr.lateral_force_N,
                .rl = rl.lateral_force_N, .rr = rr.lateral_force_N };
    acc_slip_ratio = { .fl = fl.slip_ratio, .fr = fr.slip_ratio,
                       .rl = rl.slip_ratio, .rr = rr.slip_ratio };
    acc_slip_angle = { .fl = fl.slip_angle_rad, .fr = fr.slip_angle_rad,
                       .rl = rl.slip_angle_rad, .rr = rr.slip_angle_rad };
}

} // namespace

//------------------------------------- TORQUE VECTORING UPDATE CYCLE -------------------------//

void update(const VehicleState& state, const float pedal_percentage,
            const float fl_omega, const float fr_omega,
            const float rl_omega, const float rr_omega)
{
    //------------------------------------- STATE ESTIMATION  --------------------------------//

    const auto state_estimate = vehicle_state_estimator.estimate({
        .measured_state = state,
        .wheel_angular_velocities_radps = { .fl = fl_omega, .fr = fr_omega, .rl = rl_omega, .rr = rr_omega },
        .longitudinal_forces_N = acc_f_x,
        .lateral_forces_N      = acc_f_y,
    });

    const VehicleState estimated_state = state_estimate.vehicle_state;
    est_yaw_moment_nm                  = state_estimate.yaw_moment_nm;

    // Normal forces from longitudinal/lateral load transfer + downforce
    acc_f_z = vehicle_dynamics_estimator.estimateNormalForce_N(
        estimated_state.a_x_mps2, estimated_state.a_y_mps2, estimated_state.v_x_mps);

    // Tire model estimation from current sensor data using the filtered vehicle state.
    const auto fl_out = fl_tire_model.estimate(buildTireInputs(estimated_state, fl_omega, acc_f_z.fl));
    const auto fr_out = fr_tire_model.estimate(buildTireInputs(estimated_state, fr_omega, acc_f_z.fr));
    const auto rl_out = rl_tire_model.estimate(buildTireInputs(estimated_state, rl_omega, acc_f_z.rl));
    const auto rr_out = rr_tire_model.estimate(buildTireInputs(estimated_state, rr_omega, acc_f_z.rr));
    unpackTireOutputs(fl_out, fr_out, rl_out, rr_out);

    const float acc_yaw_moment_nm = vehicle_dynamics_estimator.estimateYawMoment_Nm(
        acc_f_x, acc_f_y, estimated_state.steer_ang_rad);

    //------------------------------------- HIGH LEVEL CONTROLLER ----------------------------//

    // Desired per-wheel longitudinal force from pedal request, projected along tire heading
    const float per_wheel_tq = MAX_TORQUE_REQUEST_NM * pedal_percentage;
    wheel_set des_f_x = {
        .fl = per_wheel_tq * std::cos(acc_slip_angle.fl) * WHEEL_RADIUS_M ,
        .fr = per_wheel_tq * std::cos(acc_slip_angle.fr) * WHEEL_RADIUS_M,
        .rl = per_wheel_tq * std::cos(acc_slip_angle.rl) * WHEEL_RADIUS_M,
        .rr = per_wheel_tq * std::cos(acc_slip_angle.rr) * WHEEL_RADIUS_M,
    };

    // Direct yaw rate control: corrective yaw moment
    const float des_yaw_moment_nm = controllers::dyrc::computeYawMoment(
        estimated_state.yaw_rate_radps, estimated_state.steer_ang_rad, estimated_state.v_x_mps);

    // Distribute yaw moment into per-wheel force adjustment
    // From Mz = (t/2) * (Fx_right - Fx_left), solve for delta Fx per side:
    const float des_fx_delta = des_yaw_moment_nm / TRACK_WIDTH_m;
    des_f_x.fl -= des_fx_delta;
    des_f_x.fr += des_fx_delta;
    des_f_x.rl -= des_fx_delta;
    des_f_x.rr += des_fx_delta;

    //------------------------------------- LOW LEVEL CONTROLLER -----------------------------//

    // Gauss-Newton optimizer: find slip ratios that minimize combined force + moment tracking error
    slip_ratio_opt = torque_allocator.optimize(
        des_f_x, acc_f_z, acc_slip_ratio, estimated_state, acc_yaw_moment_nm, des_yaw_moment_nm);

    //------------------------------------- POWER LIMITER -----------------------------------//

    // TODO: slip_ratio_opt -> slipRatioToWheelAngularVelocity() -> power limiter -> torque request
}

} // namespace app::tv
