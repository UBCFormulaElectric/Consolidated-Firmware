#include <algorithm>
#include <cmath>
#include <cstdint>
#include <tuple>

#include "app_canUtils.hpp"
#include "app_sbgEllipse.hpp"

#include "util_units.hpp"

#include "velocity_estimator.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"

using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::estimators::velocity_estimator
{
static constexpr float time_step  = 0.01f;
static constexpr float SLIP_THRES = 0.0117f; // TODO: tune this
static constexpr float VX_MIN_MPS = 0.5f;    // TODO: we should probably make a global threshold in constants.hpp

// System/Process Model
autodiff::dual velocity_state_x(const EkfStateInp<float> &x)
{
    const autodiff::dual &vx  = x(0);
    const autodiff::dual &vy  = x(1);
    const autodiff::dual &ax  = x(2);
    const autodiff::dual &yaw = x(4);
    return vx + time_step * (ax - vy * yaw);
}

autodiff::dual velocity_state_y(const EkfStateInp<float> &x)
{
    const autodiff::dual &vx  = x(0);
    const autodiff::dual &vy  = x(1);
    const autodiff::dual &ay  = x(3);
    const autodiff::dual &yaw = x(4);
    return vy + time_step * (ay + vx * yaw);
}

template <Decimal T> ProcessNoiseCov<T> Q = ProcessNoiseCov<T>::Identity() * static_cast<T>(0.001);

template <Decimal T>
typename VelocityEstimator<T>::PredictStep system_model = { { velocity_state_x, velocity_state_y } };

/**
 * Measurement Model
 *
 * Note:
 *
 * Wheel speed measurements are converted into body speed then passed into the EKF
 * so the measurement model is trivial
 *
 * GPS measurements by default are in body velocity so its measurement model is also
 * trivial
 */
autodiff::dual velocity_meas_x(const EkfState<float> &x)
{
    return x(0);
}

autodiff::dual velocity_meas_y(const EkfState<float> &x)
{
    return x(1);
}

template <Decimal T> WsNoiseCov<T>  R_ws  = WsNoiseCov<T>::Identity() * static_cast<T>(0.01);
template <Decimal T> GpsNoiseCov<T> R_gps = GpsNoiseCov<T>::Identity() * static_cast<T>(0.1);

template <Decimal T>
typename VelocityEstimator<T>::UpdateSteps update_steps = typename VelocityEstimator<T>::UpdateSteps{
    WsUpdateStep<T>{ .h = { { velocity_meas_x, velocity_meas_y } }, .R = R_ws<T> },
    GpsUpdateStep<T>{ .h = { { velocity_meas_x, velocity_meas_y } }, .R = R_gps<T> },
};

// Velocity Estimator EKF instantiation
template <Decimal T> VelocityEstimator<T> velocity_estimator(system_model<T>, Q<T>, update_steps<T>);

// Helper functions for converting wheel velocity into body velocity
namespace detail
{
    template <Decimal T> wheel_set<T> motorRpmToWheelSpeedMps(const wheel_set<T> &rpm)
    {
        const T rpm_to_mps = static_cast<T>(MOTOR_RPM_TO_MPS(1.0f));
        return {
            .fl = rpm.fl * rpm_to_mps,
            .fr = rpm.fr * rpm_to_mps,
            .rl = rpm.rl * rpm_to_mps,
            .rr = rpm.rr * rpm_to_mps,
        };
    }

    template <Decimal T>
    wheel_set<Pair<T>> computeWheelVelocities(
        const wheel_set<T> &wheel_speeds_mps,
        const wheel_set<T> &steer_angles_rad,
        const T             yaw_rate_radps)
    {
        return {
            .fl = { wheel_speeds_mps.fl * std::cos(steer_angles_rad.fl) - yaw_rate_radps * HALF_TRACK_m,
                    yaw_rate_radps * DIST_FRONT_AXLE_CG_m - wheel_speeds_mps.fl * std::sin(steer_angles_rad.fl) },
            .fr = { wheel_speeds_mps.fr * std::cos(steer_angles_rad.fr) + yaw_rate_radps * HALF_TRACK_m,
                    yaw_rate_radps * DIST_FRONT_AXLE_CG_m - wheel_speeds_mps.fr * std::sin(steer_angles_rad.fr) },
            .rl = { wheel_speeds_mps.rl - yaw_rate_radps * HALF_TRACK_m, -yaw_rate_radps * DIST_REAR_AXLE_CG_m },
            .rr = { wheel_speeds_mps.rr + yaw_rate_radps * HALF_TRACK_m, -yaw_rate_radps * DIST_REAR_AXLE_CG_m },
        };
    }

    template <Decimal T> T computeSlipRatio(T vx_wheel, T vx_state)
    {
        return (vx_wheel - vx_state) / (std::max)(static_cast<T>(VX_MIN_MPS), std::abs(vx_state));
    }

    template <Decimal T>
    std::optional<WsMeasurement<T>> averageValidWheels(const wheel_set<Pair<T>> &wheel_velocities_mps, const T vx_state)
    {
        T        sum_vx      = T{ 0 };
        T        sum_vy      = T{ 0 };
        uint32_t valid_count = 0;

        const auto sum_if_valid = [&](const Pair<T> &wheel_velocity_mps)
        {
            if (std::abs(computeSlipRatio(wheel_velocity_mps.x, vx_state)) >= static_cast<T>(SLIP_THRES))
                return;

            sum_vx += wheel_velocity_mps.x;
            sum_vy += wheel_velocity_mps.y;
            valid_count++;
        };

        sum_if_valid(wheel_velocities_mps.fl);
        sum_if_valid(wheel_velocities_mps.fr);
        sum_if_valid(wheel_velocities_mps.rl);
        sum_if_valid(wheel_velocities_mps.rr);

        if (valid_count == 0)
            return std::nullopt;

        WsMeasurement<T> z;
        z << sum_vx / static_cast<T>(valid_count), sum_vy / static_cast<T>(valid_count);
        return z;
    }
} // namespace detail

// Wheel speed measurement handling
template <Decimal T>
static std::optional<WsMeasurement<T>> wheelSpeedToBodyVelocity(const VelocityEstimatorInputs<T> &inputs)
{
    const wheel_set<T> wheel_speeds_mps = detail::motorRpmToWheelSpeedMps(inputs.wheels.rpm);
    const T speed_sum_mps = wheel_speeds_mps.fl + wheel_speeds_mps.fr + wheel_speeds_mps.rl + wheel_speeds_mps.rr;

    if (speed_sum_mps <= T{ 0 })
        return WsMeasurement<T>::Zero();

    const wheel_set<Pair<T>> wheel_velocities_mps =
        detail::computeWheelVelocities(wheel_speeds_mps, inputs.wheels.steer_angles_rad, inputs.control.r_rads);
    const T vx_state = velocity_estimator<T>.state()(0);

    return detail::averageValidWheels(wheel_velocities_mps, vx_state);
}

// GPS measurement handling
template <Decimal T> static std::optional<GpsMeasurement<T>> gpsMeasurement(const VelocityEstimatorInputs<T> &inputs)
{
    if (app::sbgEllipse::getEkfSolutionMode() != app::can_utils::VcEkfStatus::POSITION)
        return std::nullopt;

    GpsMeasurement<T> z;
    z << inputs.gps.body_vx_mps, inputs.gps.body_vy_mps;
    return z;
}

// Estimation entry point
template <Decimal T> Pair<T> estimate_body_velocity(const VelocityEstimatorInputs<T> &inputs)
{
    const auto ws_meas  = wheelSpeedToBodyVelocity(inputs);
    const auto gps_meas = gpsMeasurement(inputs);

    typename VelocityEstimator<T>::Measurements measurements = std::make_tuple(ws_meas, gps_meas);

    Input<T> u;
    u << inputs.control.ax_mps2, inputs.control.ay_mps2, inputs.control.r_rads;

    const auto velocity = velocity_estimator<T>.estimated_states(u, measurements);

    return Pair<T>{ velocity(0), velocity(1) };
}

template Pair<float>  estimate_body_velocity(const VelocityEstimatorInputs<float> &inputs);
template Pair<double> estimate_body_velocity(const VelocityEstimatorInputs<double> &inputs);

template wheel_set<float>  detail::motorRpmToWheelSpeedMps(const wheel_set<float> &rpm);
template wheel_set<double> detail::motorRpmToWheelSpeedMps(const wheel_set<double> &rpm);
template wheel_set<Pair<float>>
    detail::computeWheelVelocities(const wheel_set<float> &, const wheel_set<float> &, float);
template wheel_set<Pair<double>>
                detail::computeWheelVelocities(const wheel_set<double> &, const wheel_set<double> &, double);
template float  detail::computeSlipRatio(float vx_wheel, float vx_state);
template double detail::computeSlipRatio(double vx_wheel, double vx_state);
template std::optional<WsMeasurement<float>>  detail::averageValidWheels(const wheel_set<Pair<float>> &, float);
template std::optional<WsMeasurement<double>> detail::averageValidWheels(const wheel_set<Pair<double>> &, double);
} // namespace app::tv::estimators::velocity_estimator
