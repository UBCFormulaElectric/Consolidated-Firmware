#include "vehicle_state_estimator.hpp"

#include <array>
#include <cmath>

#include "torque_vectoring/shared_datatypes/constants.hpp"

using namespace app::tv::datatypes::vd_constants;

namespace app::tv::estimation
{
namespace
{
using Filter       = VehicleStateEstimator::Filter;
using StateInput   = Filter::state_inp_mtx;
using State        = Filter::state_mtx;
using StateVector  = Filter::N_1;
using InputVector  = Filter::U_1;
using Measurement  = Filter::M_1;

static constexpr std::size_t VX = 0;
static constexpr std::size_t VY = 1;
static constexpr std::size_t R  = 2;
static constexpr std::size_t MZ = 3;

static constexpr std::size_t AX = 0;
static constexpr std::size_t AY = 1;

static constexpr float ESTIMATOR_DT_S        = 0.01f;   // Matches the 100 Hz control task.
static constexpr float ESTIMATOR_YAW_INERTIA = 110.0f;  // TODO: Replace with measured Hexray yaw inertia.

[[nodiscard]] autodiff::dual stateTransitionVx(const StateInput& x)
{
    const autodiff::dual& v_x = x(static_cast<Eigen::Index>(VX));
    const autodiff::dual& v_y = x(static_cast<Eigen::Index>(VY));
    const autodiff::dual& r   = x(static_cast<Eigen::Index>(R));
    const autodiff::dual& a_x = x(static_cast<Eigen::Index>(4 + AX));

    return v_x + (ESTIMATOR_DT_S * (a_x + (v_y * r)));
}

[[nodiscard]] autodiff::dual stateTransitionVy(const StateInput& x)
{
    const autodiff::dual& v_x = x(static_cast<Eigen::Index>(VX));
    const autodiff::dual& v_y = x(static_cast<Eigen::Index>(VY));
    const autodiff::dual& r   = x(static_cast<Eigen::Index>(R));
    const autodiff::dual& a_y = x(static_cast<Eigen::Index>(4 + AY));

    return v_y + (ESTIMATOR_DT_S * (a_y - (v_x * r)));
}

[[nodiscard]] autodiff::dual stateTransitionYawRate(const StateInput& x)
{
    const autodiff::dual& r  = x(static_cast<Eigen::Index>(R));
    const autodiff::dual& mz = x(static_cast<Eigen::Index>(MZ));

    return r + (ESTIMATOR_DT_S * (mz / ESTIMATOR_YAW_INERTIA));
}

[[nodiscard]] autodiff::dual stateTransitionYawMoment(const StateInput& x)
{
    return x(static_cast<Eigen::Index>(MZ));
}

[[nodiscard]] autodiff::dual measurementVx(const State& x)
{
    return x(static_cast<Eigen::Index>(VX));
}

[[nodiscard]] autodiff::dual measurementVy(const State& x)
{
    return x(static_cast<Eigen::Index>(VY));
}

[[nodiscard]] autodiff::dual measurementYawRate(const State& x)
{
    return x(static_cast<Eigen::Index>(R));
}

[[nodiscard]] autodiff::dual measurementYawMoment(const State& x)
{
    return x(static_cast<Eigen::Index>(MZ));
}

[[nodiscard]] auto createStateFunctions() -> std::array<Filter::StateFunction, 4>
{
    return { {
        stateTransitionVx,
        stateTransitionVy,
        stateTransitionYawRate,
        stateTransitionYawMoment,
    } };
}

[[nodiscard]] auto createMeasurementFunctions() -> std::array<Filter::MeasurementFunction, 4>
{
    return { {
        measurementVx,
        measurementVy,
        measurementYawRate,
        measurementYawMoment,
    } };
}

[[nodiscard]] auto processNoise() -> Filter::N_N
{
    Filter::N_N q = Filter::N_N::Zero();
    q(static_cast<Eigen::Index>(VX), static_cast<Eigen::Index>(VX)) = 0.05f;
    q(static_cast<Eigen::Index>(VY), static_cast<Eigen::Index>(VY)) = 0.05f;
    q(static_cast<Eigen::Index>(R), static_cast<Eigen::Index>(R))   = 0.10f;
    q(static_cast<Eigen::Index>(MZ), static_cast<Eigen::Index>(MZ)) = 150.0f;
    return q;
}

[[nodiscard]] auto measurementNoise() -> Filter::M_M
{
    Filter::M_M r = Filter::M_M::Zero();
    r(static_cast<Eigen::Index>(VX), static_cast<Eigen::Index>(VX)) = 0.75f;
    r(static_cast<Eigen::Index>(VY), static_cast<Eigen::Index>(VY)) = 0.75f;
    r(static_cast<Eigen::Index>(R), static_cast<Eigen::Index>(R))   = 0.05f;
    r(static_cast<Eigen::Index>(MZ), static_cast<Eigen::Index>(MZ)) = 250.0f;
    return r;
}

[[nodiscard]] auto initialState() -> StateVector
{
    return StateVector::Zero();
}

[[nodiscard]] auto initialCovariance() -> Filter::N_N
{
    Filter::N_N p0 = Filter::N_N::Identity();
    p0(static_cast<Eigen::Index>(VX), static_cast<Eigen::Index>(VX)) = 5.0f;
    p0(static_cast<Eigen::Index>(VY), static_cast<Eigen::Index>(VY)) = 5.0f;
    p0(static_cast<Eigen::Index>(R), static_cast<Eigen::Index>(R))   = 1.0f;
    p0(static_cast<Eigen::Index>(MZ), static_cast<Eigen::Index>(MZ)) = 400.0f;
    return p0;
}

[[nodiscard]] float readMeasuredLongitudinalAccelMps2(const VehicleStateEstimator::Inputs& inputs)
{
    // TODO: Replace with app_canRx_* when the Hexray signal table is ready.
    return inputs.measured_state.a_x_mps2;
}

[[nodiscard]] float readMeasuredLateralAccelMps2(const VehicleStateEstimator::Inputs& inputs)
{
    // TODO: Replace with app_canRx_* when the Hexray signal table is ready.
    return inputs.measured_state.a_y_mps2;
}

[[nodiscard]] float readMeasuredYawRateRadps(const VehicleStateEstimator::Inputs& inputs)
{
    // TODO: Replace with app_canRx_* when the Hexray signal table is ready.
    return inputs.measured_state.yaw_rate_radps;
}

[[nodiscard]] float readMeasuredSteeringAngleRad(const VehicleStateEstimator::Inputs& inputs)
{
    // TODO: Replace with app_canRx_* when the Hexray signal table is ready.
    return inputs.measured_state.steer_ang_rad;
}

[[nodiscard]] datatypes::datatypes::wheel_set readWheelAngularVelocitiesRadps(
    const VehicleStateEstimator::Inputs& inputs)
{
    // TODO: Replace with app_canRx_* when the Hexray signal table is ready.
    return inputs.wheel_angular_velocities_radps;
}

[[nodiscard]] Measurement pseudoMeasurementFromWheelSpeeds(
    const datatypes::datatypes::wheel_set& wheel_angular_velocities_radps,
    const float yaw_rate_radps,
    const float steering_angle_rad,
    const StateVector& previous_state)
{
    const float front_cos = std::cos(steering_angle_rad);
    const float front_sin = std::sin(steering_angle_rad);
    const float half_track_m = TRACK_WIDTH_m * 0.5f;

    const std::array<float, 4> wheel_surface_speeds_mps = {
        wheel_angular_velocities_radps.fl * WHEEL_RADIUS_M,
        wheel_angular_velocities_radps.fr * WHEEL_RADIUS_M,
        wheel_angular_velocities_radps.rl * WHEEL_RADIUS_M,
        wheel_angular_velocities_radps.rr * WHEEL_RADIUS_M,
    };

    const float speed_sum_mps =
        std::fabs(wheel_surface_speeds_mps[0]) + std::fabs(wheel_surface_speeds_mps[1]) +
        std::fabs(wheel_surface_speeds_mps[2]) + std::fabs(wheel_surface_speeds_mps[3]);

    Measurement z = Measurement::Zero();
    z(static_cast<Eigen::Index>(VX)) = previous_state(static_cast<Eigen::Index>(VX));
    z(static_cast<Eigen::Index>(VY)) = previous_state(static_cast<Eigen::Index>(VY));

    if (speed_sum_mps <= SMALL_EPSILON)
    {
        z(static_cast<Eigen::Index>(VX)) = 0.0f;
        z(static_cast<Eigen::Index>(VY)) = 0.0f;
        return z;
    }

    const float fl_vx = (wheel_surface_speeds_mps[0] * front_cos) + (yaw_rate_radps * half_track_m);
    const float fr_vx = (wheel_surface_speeds_mps[1] * front_cos) - (yaw_rate_radps * half_track_m);
    const float rl_vx = wheel_surface_speeds_mps[2] + (yaw_rate_radps * half_track_m);
    const float rr_vx = wheel_surface_speeds_mps[3] - (yaw_rate_radps * half_track_m);

    const float fl_vy = (wheel_surface_speeds_mps[0] * front_sin) - (yaw_rate_radps * DIST_FRONT_AXLE_CG_m);
    const float fr_vy = (wheel_surface_speeds_mps[1] * front_sin) - (yaw_rate_radps * DIST_FRONT_AXLE_CG_m);
    const float rl_vy = yaw_rate_radps * DIST_REAR_AXLE_CG_m;
    const float rr_vy = yaw_rate_radps * DIST_REAR_AXLE_CG_m;

    z(static_cast<Eigen::Index>(VX)) = 0.25f * (fl_vx + fr_vx + rl_vx + rr_vx);
    z(static_cast<Eigen::Index>(VY)) = 0.25f * (fl_vy + fr_vy + rl_vy + rr_vy);
    return z;
}
} // namespace

VehicleStateEstimator::VehicleStateEstimator()
    : filter_(createFilter())
{
}

auto VehicleStateEstimator::createFilter() -> Filter
{
    return Filter(
        createStateFunctions(),
        createMeasurementFunctions(),
        processNoise(),
        measurementNoise(),
        initialState(),
        initialCovariance());
}

VehicleStateEstimator::Outputs VehicleStateEstimator::estimate(const Inputs& inputs)
{
    InputVector u = InputVector::Zero();
    u(static_cast<Eigen::Index>(AX)) = readMeasuredLongitudinalAccelMps2(inputs);
    u(static_cast<Eigen::Index>(AY)) = readMeasuredLateralAccelMps2(inputs);

    const float measured_yaw_rate_radps   = readMeasuredYawRateRadps(inputs);
    const float measured_steering_angle   = readMeasuredSteeringAngleRad(inputs);
    const auto  wheel_angular_velocities  = readWheelAngularVelocitiesRadps(inputs);
    const auto& previous_state            = filter_.state();

    Measurement z = pseudoMeasurementFromWheelSpeeds(
        wheel_angular_velocities,
        measured_yaw_rate_radps,
        measured_steering_angle,
        previous_state);

    z(static_cast<Eigen::Index>(R))  = measured_yaw_rate_radps;
    z(static_cast<Eigen::Index>(MZ)) = dynamics_estimator_.estimateYawMoment_Nm(
        inputs.longitudinal_forces_N,
        inputs.lateral_forces_N,
        measured_steering_angle);

    const StateVector estimated_state = filter_.estimated_states(u, z);

    outputs_.vehicle_state = {
        .v_x_mps        = estimated_state(static_cast<Eigen::Index>(VX)),
        .v_y_mps        = estimated_state(static_cast<Eigen::Index>(VY)),
        .yaw_rate_radps = estimated_state(static_cast<Eigen::Index>(R)),
        .steer_ang_rad  = measured_steering_angle,
        .a_x_mps2       = u(static_cast<Eigen::Index>(AX)),
        .a_y_mps2       = u(static_cast<Eigen::Index>(AY)),
    };
    outputs_.yaw_moment_nm = estimated_state(static_cast<Eigen::Index>(MZ));

    return outputs_;
}

const VehicleStateEstimator::Outputs& VehicleStateEstimator::outputs() const
{
    return outputs_;
}

const VehicleStateEstimator::Covariance& VehicleStateEstimator::covariance() const
{
    return filter_.covariance();
}

void VehicleStateEstimator::reset()
{
    filter_   = createFilter();
    outputs_ = {};
}
} // namespace app::tv::estimation
