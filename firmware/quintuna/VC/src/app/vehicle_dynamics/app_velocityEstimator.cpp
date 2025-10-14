#include "app_math.hpp"
#include "app_velocityEstimator.hpp"

extern "C"
{
#include "app_units.h"
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
}

using namespace app::math;

namespace app::vehicleDynamics
{
static constexpr bool  ENABLE_GPS_UPDATE          = true;
static constexpr bool  ENABLE_WHEEL_SPEED_UPDATE  = true;
static constexpr float SPEED_CHANGE_THRESHOLD_RPM = 1000.0f;
static VelocityEstimator estimator;

/**
 * Temporary C wrapper functions
 */
extern "C"
{
void app_velocityEstimator_step(VelocityEstimator_Inputs *inputs)
{
    estimator.step(*inputs);
}
}

void VelocityEstimator::step(VelocityEstimator_Inputs &inputs)
{
    const ControlInput u = { inputs.accel_x, inputs.accel_y, inputs.yaw_rate_rad };

    predict(u);

    if constexpr (ENABLE_WHEEL_SPEED_UPDATE)
    {
        auto ws_meas = wheelSpeedsToBodyVelocity(
            inputs.rpm_rr, inputs.rpm_rl, inputs.rpm_fr, inputs.rpm_fl, inputs.yaw_rate_rad, inputs.wheel_angle_rad,
            false);
        if (ws_meas.has_value())
            update(ws_meas.value(), R_ws);
    }

    if constexpr (ENABLE_GPS_UPDATE)
    {
        StateVector gps_meas = { inputs.gps_vx, inputs.gps_vy };
        update(gps_meas, R_gps);
    }
}

void VelocityEstimator::predict(const ControlInput &u)
{
    StateVector x_prev = x;

    // state prediction
    state_transition(x_prev, u);
    state_jacobian(x_prev, u);

    // process covariance prediction
    P = F_j * P * (~F_j) + Q;
}

void VelocityEstimator::update(const StateVector &z, const Matrix2x2 &R)
{
    // measurement prediction
    measurement_func(x);
    measurement_jacobian(x);

    // measurement covariance prediction
    Matrix2x2 S = H_j * P * (~H_j) + R;

    // kalman gain calculation
    Matrix2x2 K = P * (~H_j) * S.inverse();

    // measurement residual calculation
    StateVector r = z - z_pred;

    // corrected state estimation
    x = x + (K * r);

    // corrected covariance estimation
    P = P - (K * H_j * P);
}

const StateVector &VelocityEstimator::getVelocity() const
{
    return x;
}

const Matrix2x2 &VelocityEstimator::getCovariance() const
{
    return P;
}

void VelocityEstimator::state_transition(const StateVector &x_prev, const ControlInput &u)
{
    x[0] = x_prev[0] + dt * (u[0] + x_prev[1] * u[2]);
    x[1] = x_prev[1] + dt * (u[1] - x_prev[0] * u[2]);
}

void VelocityEstimator::state_jacobian(const StateVector &x, const ControlInput &u)
{
    /**
     * This velocity estimator does not use the state vector (x)
     * to calculate the jacobian, others estimators may need to
     */
    UNUSED(x);

    F_j(0, 0) = 1.0f;
    F_j(0, 1) = dt * u[2];
    F_j(1, 0) = -dt * u[2];
    F_j(1, 1) = 1.0f;
}

/**
 * This is redundant, I just have this for clarity and understanding purposes.
 * In a different EKF and different model, the implementation of this function
 * would probably not be as trivial as this.
 */
void VelocityEstimator::measurement_func(const StateVector &x)
{
    z_pred = x;
}

/**
 * This is redundant, I just have this for clarity and understanding purposes.
 * In a different EKF and different model, the implementation of this function
 * would probably not be as trivial as this.
 */
void VelocityEstimator::measurement_jacobian(const StateVector &x)
{
    /**
     * This velocity estimator does not use the state vector (x)
     * to calculate the jacobian, others estimators may need to
     */
    UNUSED(x);

    H_j(0, 0) = 1.0f;
    H_j(1, 1) = 1.0f;
}

std::optional<StateVector> VelocityEstimator::wheelSpeedsToBodyVelocity(
    float rpm_rr,
    float rpm_rl,
    float rpm_fr,
    float rpm_fl,
    float yaw_rate,
    float steering_angle,
    bool  use_outlier_rejection)
{
    // Store RPM values for derivative check
    WheelSpeeds rpm_current = { rpm_rr, rpm_rl, rpm_fr, rpm_fl };

    if (rpm_current.sum() <= 0.0f)
    {
        measurement_ws_prev = rpm_current;
        return StateVector{ 0.0f, 0.0f };
    }

    WheelSpeeds wheel_speeds_mps = MOTOR_RPM_TO_MPS(1.0f) * rpm_current;

    // Precompute trigonometric values
    const float cos_delta = std::cos(steering_angle);
    const float sin_delta = std::sin(steering_angle);

    // Geometric constants (assumed from vehicle_geometry.hpp)
    const float half_track = TRACK_WIDTH_m / 2.0f;
    const float dist_front = DIST_FRONT_AXLE_CG;
    const float dist_rear  = DIST_REAR_AXLE_CG;

    // Calculate velocity components for each wheel at CG
    WheelSpeeds vx_components;
    WheelSpeeds vy_components;

    // Rear right wheel
    vx_components[0] = wheel_speeds_mps[0] + yaw_rate * half_track;
    vy_components[0] = -yaw_rate * dist_rear;

    // Rear left wheel
    vx_components[1] = wheel_speeds_mps[1] - yaw_rate * half_track;
    vy_components[1] = -yaw_rate * dist_rear;

    // Front right wheel (with steering)
    vx_components[2] = wheel_speeds_mps[2] * cos_delta + yaw_rate * half_track;
    vy_components[2] = yaw_rate * dist_front - wheel_speeds_mps[2] * sin_delta;

    // Front left wheel (with steering)
    vx_components[3] = wheel_speeds_mps[3] * cos_delta - yaw_rate * half_track;
    vy_components[3] = yaw_rate * dist_front - wheel_speeds_mps[3] * sin_delta;

    // Average valid measurements
    float  sum_vx      = 0.0f;
    float  sum_vy      = 0.0f;
    size_t valid_count = 0;

    for (size_t i = 0; i < 4; i++)
    {
        bool is_valid = true;

        // Optional: Reject outliers based on RPM derivative check
        if (use_outlier_rejection)
        {
            float rpm_change = std::abs(rpm_current[i] - measurement_ws_prev[i]);
            is_valid         = (rpm_change <= SPEED_CHANGE_THRESHOLD_RPM);
        }

        if (is_valid)
        {
            sum_vx += vx_components[i];
            sum_vy += vy_components[i];
            valid_count++;
        }
    }

    // Update previous measurements (store as RPM for next iteration)
    measurement_ws_prev = rpm_current;

    // Return averaged velocity or zero if no valid measurements
    if (valid_count == 0)
        return std::nullopt;

    return StateVector{ sum_vx / static_cast<float>(valid_count), sum_vy / static_cast<float>(valid_count) };
}

} // namespace app::vehicleDynamics