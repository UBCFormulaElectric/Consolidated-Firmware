#include <gtest/gtest.h>
#include <cmath>
#include <random>
#include <array>
#include "Eigen/Dense"
#include "state_estimation/app_kalman_filter.hpp"

using app::state_estimation::kf;
using app::state_estimation::ekf;

namespace {
constexpr float kDt = 0.01f;

using EkfVelocity = ekf<float, 2, 2, 3>;
using StateInpArr = EkfVelocity::state_inp_mtx;
using StateArr = EkfVelocity::state_mtx;

autodiff::dual velocity_state_x(const StateInpArr& x)
{
    const autodiff::dual& vx = x(0);
    const autodiff::dual& vy = x(1);
    const autodiff::dual& ax = x(2);
    const autodiff::dual& ay = x(3);
    const autodiff::dual& yaw = x(4);
    return vx + kDt * (ax + vy * yaw);
}

autodiff::dual velocity_state_y(const StateInpArr& x)
{
    const autodiff::dual& vx = x(0);
    const autodiff::dual& vy = x(1);
    const autodiff::dual& ax = x(2);
    const autodiff::dual& ay = x(3);
    const autodiff::dual& yaw = x(4);
    return vy + kDt * (ay + vx * yaw);
}

autodiff::dual velocity_meas_x(const StateArr& x)
{
    return x(0);
}

autodiff::dual velocity_meas_y(const StateArr& x)
{
    return x(1);
}
} // namespace

/**
* Writing tests for a simple 1D analytical case, the idea here is most of that work can be hand computed so we can 
* verify. Kalman filters can be difficult to verify so this will serve as a sanity check for us. Thus we will use the simple case of velocity estimation here
*/

/**
* SCENARIO: Two-state system tracking position (x1) and velocity (x2) with pedal input
* 
* PHYSICS MODEL:
* - Torque per motor: Tmax * pedal_percent = 22 Nm * p
* - Total torque (4 motors): Tmax * 4 * p = 88 * p Nm
* - Force with gear ratio (10:1) and wheel radius (0.5m): F = (Torque * gear_ratio) / wheel_radius = (88 * 10 * p) / 0.5 = 1760 * p N
* - Acceleration: a = F / mass = (1760 * p) / 300 kg = 5.87 * p m/s²
*
* DISCRETE-TIME SYSTEM DYNAMICS:
* State vector: X = [x1 (position), x2 (velocity)]^T
* Input: u = pedal_percent
* Time step: dt = 0.01 seconds (100 Hz)
*
* State transition equations:
*   x1(k+1) = x1(k) + x2(k)*dt + 0.5*a*dt² = x1(k) + x2(k)*dt + 2.935*p*dt²
*   x2(k+1) = x2(k) + a*dt = x2(k) + 5.87*p*dt
*
* STATE TRANSITION MATRIX F (with dt=0.01):
*   F = [1.0  0.01]
*       [0.0  1.0 ]
*
* INPUT MATRIX B (with dt=0.01):
*   B = [0.002935]   (5.87 * dt²/2)
*       [0.0587  ]   (5.87 * dt)
*
* MEASUREMENT MODEL:
* We assume we can measure velocity directly (e.g., from wheel encoders)
* Measurement equation: z = H*X where z is the measured velocity
*   H = [0.0  1.0]   (observe only velocity, not position)
*
* COVARIANCE MATRICES:
* Q (Process Noise Covariance): Models uncertainty in the system model
*   - Represents model inaccuracies, unknown disturbances, friction, etc.
*   - Typically diagonal: Q = [[q1, 0], [0, q2]]
*   - Choose q1 ≈ 0.001 to 0.01 (position uncertainty grows slowly)
*   - Choose q2 ≈ 0.01 to 0.1 (velocity uncertainty due to acceleration noise)
*
* R (Measurement Noise Covariance): Models sensor uncertainty
*   - For direct velocity measurement: R = [r] (scalar)
*   - Typical range: R ≈ 0.01 to 0.1 depending on sensor quality
*   - If R is too small, filter trusts measurements too much and gets noisy
*   - If R is too large, filter ignores measurements and relies on prediction
*
* TEST SCENARIO:
* - Start with x1=0, x2=0 (at rest)
* - Apply pedal_percent=0.5 (50% throttle) for 10 steps
* - Simulate measurements with small noise
* - Verify filter converges to expected velocity ≈ 0.5 * 5.87 * 0.01 = 0.02935 m/s per step
*/

/**
* COMPLETE TEST EXAMPLE for the Kalman Filter scenario
* 
* This test demonstrates:
* 1. System setup with F, B, H matrices using Eigen
* 2. Covariance matrix selection (Q and R)
* 3. Running the filter over a sequence of inputs and measurements
* 4. Verifying the filter output converges to expected values
*/

TEST(KalmanFilterTest, VelocityEstimationWith50PercentThrottle)
{
    // Constants from our scenario
    const float dt = 0.01f;           // 100 Hz update rate
    const float acceleration_factor = 5.87f;  // 5.87 * pedal_percent = acceleration
    const float pedal_percent = 0.5f; // 50% throttle
    const float expected_accel = acceleration_factor * pedal_percent;  // 2.935 m/s²
    
    // ============================================
    // 1. Define State Transition Matrix F (2x2)
    // ============================================
    // F = [1.0  0.01]
    //     [0.0  1.0 ]
    Eigen::Matrix<float, 2, 2> F;
    F << 1.0f, dt,
         0.0f, 1.0f;
    
    // ============================================
    // 2. Define Input Matrix B (2x1)
    // ============================================
    // B = [0.5 * a * dt²]  = [0.002935]
    //     [a * dt      ]     [0.0587  ]
    Eigen::Matrix<float, 2, 1> B;
    B << 0.5f * acceleration_factor * dt * dt,
         acceleration_factor * dt;
    
    // ============================================
    // 3. Define Measurement Matrix H (1x2)
    // ============================================
    // H = [0.0  1.0]  - we measure velocity directly from wheel encoders
    Eigen::Matrix<float, 1, 2> H;
    H << 0.0f, 1.0f;
    
    // ============================================
    // 4. Define Process Noise Covariance Q (2x2)
    // ============================================
    // Diagonal: q_position ≈ 0.001, q_velocity ≈ 0.01
    Eigen::Matrix<float, 2, 2> Q;
    Q << 0.001f, 0.0f,
         0.0f, 0.01f;
    
    // ============================================
    // 5. Define Measurement Noise Covariance R (1x1)
    // ============================================
    Eigen::Matrix<float, 1, 1> R;
    R << 0.05f;
    
    // ============================================
    // 6. Define Initial State and Covariance
    // ============================================
    // Initial state: at rest at position 0
    Eigen::Matrix<float, 2, 1> x_estimated;
    x_estimated << 0.0f, 0.0f;  // [position, velocity]^T
    
    // Initial state covariance: uncertain about starting state
    Eigen::Matrix<float, 2, 2> P;
    P << 1.0f, 0.0f,
         0.0f, 1.0f;
    
    // ============================================
    // 7. Create and Initialize Kalman Filter
    // ============================================
    // TODO: Initialize your KF class with the matrices above
    // kf filter(F, B, H, Q, R, x_estimated, P);
    
    // ============================================
    // 8. Simulate System and Filter for 10 steps
    // ============================================
    float accumulated_velocity = 0.0f;  // Track true velocity for comparison
    std::mt19937 rng(42);  // Deterministic seed for reproducibility
    std::normal_distribution<float> noise_dist(0.0f, 0.005f);  // ±0.005 m/s noise
    
    for (int k = 0; k < 10; ++k) {
        // --- TRUE SYSTEM DYNAMICS (with noise in measurement) ---
        accumulated_velocity += expected_accel * dt;
        // Simulate sensor measurement with small noise
        float measurement_noise = noise_dist(rng);
        float measured_velocity = accumulated_velocity + measurement_noise;
        
        // --- KALMAN FILTER PREDICTION STEP ---
        // x_pred = F * x + B * u
        Eigen::Matrix<float, 2, 1> x_pred = F * x_estimated + B * pedal_percent;
        
        // P_pred = F * P * F^T + Q
        Eigen::Matrix<float, 2, 2> P_pred = F * P * F.transpose() + Q;
        
        // --- KALMAN FILTER UPDATE STEP ---
        // Using measured_velocity from wheel encoder
        Eigen::Matrix<float, 1, 1> z;
        z << measured_velocity;
        
        // y = z - H * x_pred  (measurement residual)
        Eigen::Matrix<float, 1, 1> y = z - H * x_pred;
        
        // S = H * P_pred * H^T + R  (residual covariance)
        Eigen::Matrix<float, 1, 1> S = H * P_pred * H.transpose() + R;
        
        // K = P_pred * H^T * S^-1  (Kalman gain)
        Eigen::Matrix<float, 2, 1> K = P_pred * H.transpose() * S.inverse();
        
        // x_estimated = x_pred + K * y
        x_estimated = x_pred + K * y;
        
        // P = (I - K * H) * P_pred
        Eigen::Matrix<float, 2, 2> I = Eigen::Matrix<float, 2, 2>::Identity();
        P = (I - K * H) * P_pred;
        
        // --- VERIFY BEHAVIOR ---
        SCOPED_TRACE("Iteration " + std::to_string(k));
        // Verify that estimates are reasonable
        EXPECT_LT(x_estimated(1), 1.0f);  // velocity shouldn't exceed 1 m/s in 10 steps
        EXPECT_GT(x_estimated(0), -0.1f);  // position should not go negative
    }
    
    // Final check: velocity should be close to calculated value
    float expected_final_velocity = expected_accel * dt * 10;  // ≈ 0.2935 m/s
    EXPECT_NEAR(x_estimated(1), expected_final_velocity, 0.05f);
    
    // Final check: position should have increased
    EXPECT_GT(x_estimated(0), 0.001f);
}

TEST(ExtendedKalmanFilterTest, VelocityEstimatorMockMatchesReference)
{
    using N2 = Eigen::Matrix<float, 2, 2>;
    using V2 = Eigen::Matrix<float, 2, 1>;
    using V3 = Eigen::Matrix<float, 3, 1>;

    V2 x0;
    x0 << 1.0f, 2.0f;

    N2 P0;
    P0 << 1.0f, 2.0f,
          2.0f, 1.0f;

    const N2 Q = N2::Zero();

    N2 R;
    R << 0.04f, 0.0f,
         0.0f, 0.09f;

    const std::array<EkfVelocity::StateFunction, 2> f = {{
        velocity_state_x,
        velocity_state_y,
    }};
    const std::array<EkfVelocity::MeasurementFunction, 2> h = {{
        velocity_meas_x,
        velocity_meas_y,
    }};

    EkfVelocity filter(f, h, Q, R, x0, P0);

    V3 u;
    u << 1.0f, 2.0f, 3.0f; // accel_x, accel_y, yaw_rate

    V2 z;
    z << 1.5f, 2.5f;

    // Reference predict step (matches velocity estimator equations)
    V2 x_pred;
    x_pred << x0(0) + kDt * (u(0) + x0(1) * u(2)),
              x0(1) + kDt * (u(1) + x0(0) * u(2));

    N2 F;
    F << 1.0f, kDt * u(2),
         -kDt * u(2), 1.0f;

    N2 P_pred = F * P0 * F.transpose() + Q;

    // Reference update step (H = I)
    const N2 S = P_pred + R;
    const N2 K = P_pred * S.llt().solve(N2::Identity());
    const V2 y = z - x_pred;
    const V2 x_expected = x_pred + K * y;
    const N2 P_expected = (N2::Identity() - K) * P_pred;

    V2 x_actual = filter.estimated_states(u, z);
    N2 P_actual = filter.covariance();

    for (int i = 0; i < 2; ++i)
    {
        EXPECT_NEAR(x_expected(i), x_actual(i), 1e-5f);
    }

    for (int r = 0; r < 2; ++r)
    {
        for (int c = 0; c < 2; ++c)
        {
            EXPECT_NEAR(P_expected(r, c), P_actual(r, c), 1e-5f);
        }
    }
}

TEST(ExtendedKalmanFilterTest, ZeroMeasurementNoiseSnapsToMeasurement)
{
    using N2 = Eigen::Matrix<float, 2, 2>;
    using V2 = Eigen::Matrix<float, 2, 1>;
    using V3 = Eigen::Matrix<float, 3, 1>;

    V2 x0;
    x0 << 1.0f, 2.0f;

    N2 P0;
    P0 << 1.0f, 2.0f,
          2.0f, 1.0f;

    const N2 Q = N2::Zero();
    const N2 R = N2::Zero();

    const std::array<EkfVelocity::StateFunction, 2> f = {{
        velocity_state_x,
        velocity_state_y,
    }};
    const std::array<EkfVelocity::MeasurementFunction, 2> h = {{
        velocity_meas_x,
        velocity_meas_y,
    }};

    EkfVelocity filter(f, h, Q, R, x0, P0);

    V3 u;
    u << 1.0f, 2.0f, 3.0f;

    V2 z;
    z << 1.5f, 2.5f;

    V2 x_actual = filter.estimated_states(u, z);
    N2 P_actual = filter.covariance();

    for (int i = 0; i < 2; ++i)
    {
        EXPECT_NEAR(z(i), x_actual(i), 1e-6f);
    }

    for (int r = 0; r < 2; ++r)
    {
        for (int c = 0; c < 2; ++c)
        {
            EXPECT_NEAR(0.0f, P_actual(r, c), 1e-6f);
        }
    }
}

/**
 * CHOOSING COVARIANCE MATRICES - PRACTICAL GUIDE:
 * 
 * Process Noise Covariance (Q):
 * =============================
 * Represents: How much we believe the system model
 * 
 * In our case:
 * - Position update: x_new = x_old + v*dt + 0.5*a*dt²
 *   We're pretty confident in this kinematics, so q11 should be small (0.001-0.01)
 * 
 * - Velocity update: v_new = v_old + a*dt
 *   This depends on our acceleration estimate (5.87*pedal%), which may be affected by:
 *   * Friction (varies with road surface)
 *   * Tire slip (at high acceleration)
 *   * Aerodynamic drag (quadratic with speed)
 *   So q22 should be larger (0.01-0.1)
 * 
 * Q = [[q11, 0  ],
 *      [0,   q22]]
 *
 * Measurement Noise Covariance (R):
 * ==================================
 * Represents: Sensor accuracy and uncertainty
 * 
 * In our case: Wheel encoder measuring velocity
 * - Modern encoders have ~0.1-0.5% error
 * - For velocity range 0-10 m/s, that's ±0.01-0.05 m/s noise
 * - Noise covariance R = (std_dev)² ≈ (0.025)² ≈ 0.0006 to (0.05)² ≈ 0.0025
 * - Practical range: R = 0.001 to 0.1
 * 
 * R = [r]  (1x1 matrix for single measurement)
 *
 * TUNING PROCESS:
 * ===============
 * 1. Start with Q_initial = 0.01*I, R_initial = 0.01
 * 2. Run simulation and check filter response:
 *    - If filter lags behind true value: increase Q (trust model less)
 *    - If filter oscillates around measurement: increase R (trust sensor less)
 * 3. Adjust ratio Q/R:
 *    - High ratio: Filter follows measurements closely
 *    - Low ratio: Filter follows model predictions closely
 * 4. Typical sweet spot: Q/R ≈ 0.1 to 1.0
 * 
 * EMPIRICAL METHOD:
 * ================
 * Record actual sensor data and compute:
 * - Q: Variance of (model_predicted - actual_value)
 * - R: Variance of sensor noise around true value
 */
