#include "app_velocityEstimator.hpp"

extern "C"
{
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_sbgEllipse.h"
}

using namespace app::math;

namespace app::vehicleDynamics::velocityEstimator
{
static constexpr bool  ENABLE_GPS_UPDATE          = true;
static constexpr bool  ENABLE_WHEEL_SPEED_UPDATE  = true;
static constexpr float SPEED_CHANGE_THRESHOLD_RPM = 1000.0f;

/**
 * x, P, F_j, H_j, z_pred, and Q are universal for all EKFs
 *
 * Usually an EKF has a singular R, but here we have two R's (R_ws and R_gps)
 * so we can perform two update steps with gps and wheelspeed, rather than just
 * one of them
 *
 */
// State estimate vector
static StateVector x{};

// Covariance estimate
static Matrix2x2 P{};

// State transition Jacobian
static Matrix2x2 F_j{};

// Measurement Jacobian
static Matrix2x2 H_j{};

// Predicted measurement
static StateVector z_pred{};

// Process noise covariance
static const Matrix2x2 Q{ 0.02f, 0.0f, 0.0f, 0.02f };

// Measurement noise covariance for wheel speed
static const Matrix2x2 R_ws{ 0.02f, 0.0f, 0.0f, 0.02f };

// Measurement noise covariance for GPS {vx, vy}
static const Matrix2x2 R_gps{ 0.02f, 0.0f, 0.0f, 0.02f };

// Previous wheel speed measurements {rr, rl, fr, fl}
static WheelSpeeds measurement_ws_prev{};

/**
 * The functions below exist in all EKF's and their implementation depends
 * on the state space model and what you would like to estimate.
 * Thus these should be user implementable in a general EKF API
 */

/**
 * @brief Updates the state estimate vector based on the previous
 * state and control inputs
 * Equation: x_k|k-1 = f(x_{k-1|k-1}, u_k)
 * x becomes x_k|k-1
 * @param x_prev: previous state estimate x_{k-1|k-1},
 * @param u: control inputs {accel x, accel y, yaw rate} u_k
 */
static void state_transition(const StateVector &x_prev, const ControlInput &u);

static void state_jacobian(const StateVector &x_prev, const ControlInput &u);

static void measurement_func(const StateVector &x_prev);

static void measurement_jacobian(const StateVector &x_prev);

static std::optional<StateVector> wheelSpeedsToBodyVelocity(
    float rpm_rr,
    float rpm_rl,
    float rpm_fr,
    float rpm_fl,
    float yaw_rate,
    float steering_angle,
    bool  use_outlier_rejection);

void app_velocityEstimator_step(VelocityEstimator_Inputs& inputs)
{
    const ControlInput u = { inputs.accel_x, inputs.accel_y, inputs.yaw_rate_rad };

    predict(u);

    if constexpr (ENABLE_WHEEL_SPEED_UPDATE)
    {
        // Update estimate with wheelspeed
        auto ws_meas = wheelSpeedsToBodyVelocity(
            inputs.rpm_rr, inputs.rpm_rl, inputs.rpm_fr, inputs.rpm_fl, inputs.yaw_rate_rad, inputs.wheel_angle_rad,
            false);
        if (ws_meas.has_value())
            update(ws_meas.value(), R_ws);
    }

    if constexpr (ENABLE_GPS_UPDATE)
    {
        // Update estimate with GPS
        if (app_sbgEllipse_getEkfSolutionMode() == POSITION)
        {
            StateVector gps_meas = { inputs.gps_vx, inputs.gps_vy };
            update(gps_meas, R_gps);
        }
    }
}

void predict(const ControlInput &u)
{
    StateVector x_prev = x;

    // state prediction
    state_transition(x_prev, u);
    state_jacobian(x_prev, u);

    // process covariance prediction
    P = F_j * P * (~F_j) + Q;
}

void update(const StateVector &z, const Matrix2x2 &R)
{
    // measurement prediction
    measurement_func(x);
    measurement_jacobian(x);

    // measurement covariance prediction
    Matrix2x2 S = H_j * P * (~H_j) + R;

    // kalman gain calculation
    // remember to make S inverse here 
    Matrix2x2 K = P * (~H_j) * S;

    // measurement residual calculation
    StateVector r = z - z_pred;

    // corrected state estimation
    x = x + (K * r);

    // corrected covariance estimation
    P = P - (K * H_j * P);
}

void reset()
{
    x                   = {};
    P                   = {};
    F_j                 = {};
    H_j                 = {};
    measurement_ws_prev = {};
    z_pred              = {};
}

void reset(const StateVector &x_init, const Matrix2x2 &P_init)
{
    x                   = x_init;
    P                   = P_init;
    F_j                 = {};
    H_j                 = {};
    measurement_ws_prev = {};
    z_pred              = {};
}

const StateVector &getVelocity()
{
    return x;
}

const Matrix2x2 &getCovariance()
{
    return P;
}

static void state_transition(const StateVector &x_prev, const ControlInput &u)
{
    x[0] = x_prev[0] + dt * (u[0] + x_prev[1] * u[2]);
    x[1] = x_prev[1] + dt * (u[1] - x_prev[0] * u[2]);
}

static void state_jacobian(const StateVector &x_prev, const ControlInput &u)
{
    /**
     * This velocity estimator does not use the state vector (x)
     * to calculate the jacobian, others estimators may need to
     */
    UNUSED(x_prev);

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
static void measurement_func(const StateVector &x_pred)
{
    z_pred = x_pred;
}

/**
 * This is redundant, I just have this for clarity and understanding purposes.
 * In a different EKF and different model, the implementation of this function
 * would probably not be as trivial as this.
 */
static void measurement_jacobian(const StateVector &x_pred)
{
    /**
     * This velocity estimator does not use the state vector (x)
     * to calculate the jacobian, others estimators may need to
     */
    UNUSED(x_pred);

    H_j(0, 0) = 1.0f;
    H_j(1, 1) = 1.0f;
}

static std::optional<StateVector> wheelSpeedsToBodyVelocity(
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

    WheelSpeeds wheel_speeds_mps = (float)MOTOR_RPM_TO_MPS(1.0f) * rpm_current;

    // Precompute trigonometric values
    const float cos_delta = std::cos(steering_angle);
    const float sin_delta = std::sin(steering_angle);

    // Geometric constants (assumed from vehicle_geometry.hpp)
    const float half_track = TRACK_WIDTH_m / 2.0f;
    const float dist_front = DIST_FRONT_AXLE_CG;
    const float dist_rear  = DIST_REAR_AXLE_CG;

    // Calculate velocity components for each wheel at CG
    WheelSpeeds vx_components = {
        wheel_speeds_mps[0] + yaw_rate * half_track,            
        wheel_speeds_mps[1] - yaw_rate * half_track,            
        wheel_speeds_mps[2] * cos_delta + yaw_rate * half_track,
        wheel_speeds_mps[3] * cos_delta - yaw_rate * half_track 
    };

    WheelSpeeds vy_components = {
        -yaw_rate * dist_rear,                                  
        -yaw_rate * dist_rear,                                  
        yaw_rate * dist_front - wheel_speeds_mps[2] * sin_delta,
        yaw_rate * dist_front - wheel_speeds_mps[3] * sin_delta 
    };

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

} // namespace app::vehicleDynamics::velocityEstimator