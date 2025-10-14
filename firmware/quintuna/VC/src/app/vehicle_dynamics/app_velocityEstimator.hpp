#pragma once

#include <optional>
#include "app_math.hpp"

/**
 * Discrete Time Extended Kalman Filter:
 * https://en.wikipedia.org/wiki/Extended_Kalman_filter
 */

namespace app::vehicleDynamics
{
using namespace app::math;
static constexpr size_t DIM = 2u;
static constexpr float  dt  = 0.01f;

// Type aliases for different matrix shapes
using StateVector  = matrix<float, DIM, 1>;
using Matrix2x2    = matrix<float, DIM, DIM>;
using WheelSpeeds  = matrix<float, 4, 1>;
using ControlInput = matrix<float, 3, 1>;
using Vec4         = matrix<float, 4, 1>;

class VelocityEstimator
{
  private:
    /**
     * x, P, F_j, H_j, z_pred, and Q are universal for all EKFs
     *
     * Usually an EKF has a singular R, but here we have two R's (R_ws and R_gps)
     * so we can perform two update steps with gps and wheelspeed, rather than just
     * one of them
     *
     */
    // State estimate vector
    StateVector x{};

    // Covariance estimate
    Matrix2x2 P{};

    // State transition Jacobian
    Matrix2x2 F_j{};

    // Measurement Jacobian
    Matrix2x2 H_j{};

    // Predicted measurement
    StateVector z_pred{};

    // Process noise covariance
    const Matrix2x2 Q{};

    // Measurement noise covariance for wheel speed
    const Matrix2x2 R_ws{};

    // Measurement noise covariance for GPS {vx, vy}
    const Matrix2x2 R_gps{};

    /**
     * The functions below (state transition, state jacobian, measurement func,
     * measurement jacobian) are also universal in EKFs and their implementation
     * depends on your model and what you are estimating.
     */

    /**
     * @brief Updates the state estimate vector based on the previous
     * state and control inputs
     * Equation: x_k|k-1 = f(x_{k-1|k-1}, u_k)
     * x becomes x_k|k-1
     * @param x_prev: previous state estimate x_{k-1|k-1},
     * @param u: control inputs {accel x, accel y, yaw rate} u_k
     */
    void state_transition(const StateVector &x_prev, const ControlInput &u);

    void state_jacobian(const StateVector &x, const ControlInput &u);

    void measurement_func(const StateVector &x);

    void measurement_jacobian(const StateVector &x);

    std::optional<StateVector> wheelSpeedsToBodyVelocity(
        float rpm_rr,
        float rpm_rl,
        float rpm_fr,
        float rpm_fl,
        float yaw_rate,
        float steering_angle,
        bool  use_outlier_rejection);

    // Previous wheel speed measurements {rr, rl, fr, fl}
    WheelSpeeds measurement_ws_prev{};

  public:
    typedef struct
    {
        // measurements
        // wheel speeds
        float rpm_rr;
        float rpm_rl;
        float rpm_fr;
        float rpm_fl;
        // steering
        float wheel_angle_rad;
        // gps
        float gps_vx;
        float gps_vy;
        // control inputs
        float accel_x;
        float accel_y;
        float yaw_rate_rad;
    } VelocityEstimator_Inputs;

    /**
     * @brief Initialize the velocity estimator
     * @param None for now (purely for convenience purposes)
     * will add in actual configuration arguments later
     */
    explicit VelocityEstimator() = default;

    /**
     * @brief Run the predict and update steps together
     * @param inputs: Sensor inputs containing
     *          [0]: longitudinal acceleration (m/s²)
     *          [1]: lateral acceleration (m/s²)
     *          [2]: yaw rate (rad/s)
     */
    void step(VelocityEstimator_Inputs &inputs);

    /**
     * @brief Prediction step - projects state forward one time step
     *
     * Uses the process model to predict the next state from the current state
     * estimate and control inputs. Updates both the state vector (x) and
     * covariance matrix (P).
     *
     * Equations:
     *   x_pred = f(x, u)           // Predict state
     *   P_pred = F*P*F^T + Q       // Predict covariance
     *
     * Where:
     *   x: current state estimate [vx, vy] (updated to predicted state)
     *   u: control input [ax, ay, yaw_rate]
     *   F: state transition Jacobian (linearized process model)
     *   P: state covariance matrix (updated to predicted covariance)
     *   Q: process noise covariance
     *
     * @param u Control input vector containing:
     *          [0]: longitudinal acceleration (m/s²)
     *          [1]: lateral acceleration (m/s²)
     *          [2]: yaw rate (rad/s)
     */
    void predict(const ControlInput &u);

    /**
     * @brief Update (correction) step - corrects state estimate with measurement
     *
     * Incorporates sensor measurements to correct the predicted state estimate
     * and reduce uncertainty. This is the measurement fusion step where external
     * observations (GPS, wheel speeds, etc.) are used to update the filter.
     * Updates both the state vector (x) and covariance matrix (P).
     *
     * Equations:
     *   z_pred = h(x)              // Predict measurement from current state
     *   H = ∂h/∂x                  // Measurement Jacobian (linearization)
     *   S = H*P*H^T + R            // Innovation covariance
     *   K = P*H^T*S^-1             // Kalman gain (optimal weight)
     *   r = z - z_pred             // Innovation/residual
     *   x = x + K*r                // Corrected state estimate
     *   P = P - K*H*P              // Corrected covariance estimate
     *
     * Where:
     *   x: predicted state estimate [vx, vy] (updated to corrected state)
     *   z: actual sensor measurement [vx_measured, vy_measured]
     *   z_pred: predicted measurement based on current state
     *   H: measurement Jacobian (identity matrix for this estimator)
     *   P: state covariance matrix (updated to corrected covariance)
     *   R: measurement noise covariance (sensor-specific uncertainty)
     *   S: innovation covariance (predicted measurement uncertainty)
     *   K: Kalman gain (determines how much to trust measurement vs. prediction)
     *   r: innovation/residual (difference between actual and predicted measurement)
     *
     * @param z Measurement vector containing:
     *          [0]: measured longitudinal velocity vx (m/s)
     *          [1]: measured lateral velocity vy (m/s)
     * @param R Measurement noise covariance matrix (2x2)
     *          Diagonal elements represent measurement uncertainty
     *          Use R_ws for wheel speed sensors, R_gps for GPS
     */
    void               update(const StateVector &z, const Matrix2x2 &R);
    void               reset();
    void               reset(const StateVector &x_init, const Matrix2x2 &P_init);
    const StateVector &getVelocity() const;
    const Matrix2x2   &getCovariance() const;
};
} // namespace app::vehicleDynamics

