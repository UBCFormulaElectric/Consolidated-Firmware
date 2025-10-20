#pragma once

#include <optional>
#include "../../../shared/srcpp/app/app_math.hpp"



/**
 * Discrete Time Extended Kalman Filter:
 * https://en.wikipedia.org/wiki/Extended_Kalman_filter
 */

namespace app::vehicleDynamics::velocityEstimator
{
static constexpr size_t DIM = 2u;
static constexpr float  dt  = 0.01f;

// Type aliases for different matrix shapes
using StateVector  = app::math::matrix<float, DIM, 1>;
using Matrix2x2    = app::math::matrix<float, DIM, DIM>;
using WheelSpeeds  = app::math::matrix<float, 4, 1>;
using ControlInput = app::math::matrix<float, 3, 1>;
using Vec4         = app::math::matrix<float, 4, 1>;

// typedef enum
// {

// } VelocityEstimator_State;

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
 * @brief Run the predict and update steps together
 * @param inputs: Sensor inputs containing
 */
void app_velocityEstimator_step(VelocityEstimator_Inputs& inputs);

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
void update(const StateVector &z, const Matrix2x2 &R);

/**
 * @brief Reset filter to zero initial conditions
 * 
 * Sets state estimate to zero velocity and resets covariance to zero.
 * Use after initialization or to recover from filter divergence.
 */
void               reset();

/**
 * @brief Reset filter with specified initial conditions
 * 
 * @param x_init Initial state estimate [vx, vy] (m/s)
 * @param P_init Initial covariance estimate (2x2 matrix)
 */
void               reset(const StateVector &x_init, const Matrix2x2 &P_init);

/**
 * @brief Get current velocity estimate
 * 
 * @return Reference to state vector [vx, vy] in body frame (m/s)
 */
const StateVector &getVelocity();

/**
 * @brief Get current estimation uncertainty
 * 
 * @return Reference to 2x2 covariance matrix representing velocity uncertainty
 */
const Matrix2x2   &getCovariance();
} // namespace app::vehicleDynamics::velocityEstimator