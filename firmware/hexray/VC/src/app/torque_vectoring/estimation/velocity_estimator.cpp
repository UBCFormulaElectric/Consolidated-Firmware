#include <cmath>

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

namespace detail
{
    Eigen::Matrix<float, NUM_WHEELS, 1>
        computeWheelVx(const Eigen::Matrix<float, NUM_WHEELS, 1> &ws_mps, float cos_fr, float cos_fl, float yaw)
    {
        Eigen::Matrix<float, NUM_WHEELS, 1> vx;
        vx << ws_mps[0] + yaw * HALF_TRACK_m, ws_mps[1] - yaw * HALF_TRACK_m, ws_mps[2] * cos_fr + yaw * HALF_TRACK_m,
            ws_mps[3] * cos_fl - yaw * HALF_TRACK_m;
        return vx;
    }

    Eigen::Matrix<float, NUM_WHEELS, 1>
        computeWheelVy(const Eigen::Matrix<float, NUM_WHEELS, 1> &ws_mps, float sin_fr, float sin_fl, float yaw)
    {
        Eigen::Matrix<float, NUM_WHEELS, 1> vy;
        vy << -yaw * DIST_REAR_AXLE_CG_m, -yaw * DIST_REAR_AXLE_CG_m, yaw * DIST_FRONT_AXLE_CG_m - ws_mps[2] * sin_fr,
            yaw * DIST_FRONT_AXLE_CG_m - ws_mps[3] * sin_fl;
        return vy;
    }

    float computeSlipRatio(float vx_wheel, float vx_state)
    {
        return (vx_wheel - vx_state) / std::max(VX_MIN_MPS, std::abs(vx_state));
    }

    std::optional<WsMeasurement> averageValidWheels(
        const Eigen::Matrix<float, NUM_WHEELS, 1> &vx_wheels,
        const Eigen::Matrix<float, NUM_WHEELS, 1> &vy_wheels,
        float                                      vx_state)
    {
        float    sum_vx      = 0.0f;
        float    sum_vy      = 0.0f;
        uint32_t valid_count = 0;

        for (uint32_t i = 0; i < NUM_WHEELS; i++)
        {
            if (std::abs(computeSlipRatio(vx_wheels[i], vx_state)) >= SLIP_THRES)
                continue;

            sum_vx += vx_wheels[i];
            sum_vy += vy_wheels[i];
            valid_count++;
        }

        if (valid_count == 0)
            return std::nullopt;

        WsMeasurement z;
        z << sum_vx / static_cast<float>(valid_count), sum_vy / static_cast<float>(valid_count);
        return z;
    }
} // namespace detail

// --------------- SYSTEM MODEL --------------- //

/**
 * Bicycle based body velocity model
 */
autodiff::dual velocity_state_x(const EkfStateInp &x)
{
    const autodiff::dual &vx  = x(0);
    const autodiff::dual &vy  = x(1);
    const autodiff::dual &ax  = x(2);
    const autodiff::dual &yaw = x(4);
    return vx + time_step * (ax - vy * yaw);
}

autodiff::dual velocity_state_y(const EkfStateInp &x)
{
    const autodiff::dual &vx  = x(0);
    const autodiff::dual &vy  = x(1);
    const autodiff::dual &ay  = x(3);
    const autodiff::dual &yaw = x(4);
    return vy + time_step * (ay + vx * yaw);
}

/**
 * For now for simplicity sake I am assuming that each state does
 * not affect each other, but in reality it this is probably the case
 * to a small extent.
 */
ProcessNoiseCov Q = ProcessNoiseCov::Identity() * 0.001f;

VelocityEstimator::PredictStep system_model = { { velocity_state_x, velocity_state_y } };

// --------------- MEASUREMENT MODELS --------------- //

/**
 * The wheelspeed and GPS measurement models are both trivial.
 *
 * The wheelspeeds are transformed into body velocity before being given
 * to the EKF, and the GPS automatically outputs body velocity, thus both
 * of their models are just identity functions.
 *
 * In reality, each update step would have different measurment models,
 * but this is a special exception.
 */
autodiff::dual velocity_meas_x(const EkfState &x)
{
    return x(0);
}

autodiff::dual velocity_meas_y(const EkfState &x)
{
    return x(1);
}

/**
 * For now for simplicity sake I am assuming that each state does
 * not affect each other, but in reality it this is probably the case
 * to a small extent.
 */
WsNoiseCov  R_ws  = WsNoiseCov::Identity() * 0.01f;
GpsNoiseCov R_gps = GpsNoiseCov::Identity() * 0.1f;

VelocityEstimator::UpdateSteps update_steps = {
    { WsUpdateStep{ .h = { { velocity_meas_x, velocity_meas_y } }, .R = R_ws } },
    { GpsUpdateStep{ .h = { { velocity_meas_x, velocity_meas_y } }, .R = R_gps } },
};

VelocityEstimator velocity_estimator(system_model, Q, update_steps);

static std::optional<WsMeasurement> wheelSpeedToBodyVelocity(const VelocityEstimatorInputs &inputs)
{
    Eigen::Matrix<float, NUM_WHEELS, 1> ws_mps;
    ws_mps << inputs.wheels.rr_rpm, inputs.wheels.rl_rpm, inputs.wheels.fr_rpm, inputs.wheels.fl_rpm;
    ws_mps *= MOTOR_RPM_TO_MPS(1.0f);

    if (ws_mps.sum() <= 0.0f)
        return WsMeasurement::Zero();

    const float cos_fr = std::cosf(inputs.wheels.ang_fr_rad);
    const float sin_fr = std::sinf(inputs.wheels.ang_fr_rad);
    const float cos_fl = std::cosf(inputs.wheels.ang_fl_rad);
    const float sin_fl = std::sinf(inputs.wheels.ang_fl_rad);
    const float yaw    = inputs.control.r_rads;

    const auto  vx_wheels = detail::computeWheelVx(ws_mps, cos_fr, cos_fl, yaw);
    const auto  vy_wheels = detail::computeWheelVy(ws_mps, sin_fr, sin_fl, yaw);
    const float vx_state  = velocity_estimator.state()(0);

    return detail::averageValidWheels(vx_wheels, vy_wheels, vx_state);
}

static std::optional<GpsMeasurement> gpsMeasurement(const VelocityEstimatorInputs &inputs)
{
    if (app::sbgEllipse::getEkfSolutionMode() == app::can_utils::VcEkfStatus::POSITION)
        return GpsMeasurement{ inputs.gps.body_vx_mps, inputs.gps.body_vy_mps };

    return std::nullopt;
}

Velocity estimate_body_velocity(const VelocityEstimatorInputs &inputs)
{
    const auto ws_meas  = wheelSpeedToBodyVelocity(inputs);
    const auto gps_meas = gpsMeasurement(inputs);

    VelocityEstimator::Measurements measurements = std::make_tuple(ws_meas, gps_meas);

    Input u;
    u << inputs.control.ax_mps2, inputs.control.ay_mps2, inputs.control.r_rads;

    return velocity_estimator.estimated_states(u, measurements);
}
} // namespace app::tv::estimators::velocity_estimator