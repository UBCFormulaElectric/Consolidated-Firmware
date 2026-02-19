#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <random>

#include "Eigen/Dense"
#include "state_estimation/app_kalman_filter.hpp"

using app::state_estimation::ekf;
using app::state_estimation::kf;

namespace {
constexpr float kTimeStep = 0.01f;

using KfVelocity = kf<float, 2, 1, 1>;
using KfN2 = Eigen::Matrix<float, 2, 2>;
using KfV2 = Eigen::Matrix<float, 2, 1>;
using KfU1 = Eigen::Matrix<float, 1, 1>;
using KfM1 = Eigen::Matrix<float, 1, 1>;
using KfH = Eigen::Matrix<float, 1, 2>;

using EkfVelocity = ekf<float, 2, 2, 3>;
using EkfN2 = Eigen::Matrix<float, 2, 2>;
using EkfV2 = Eigen::Matrix<float, 2, 1>;
using EkfU3 = Eigen::Matrix<float, 3, 1>;
using EkfStateInp = EkfVelocity::state_inp_mtx;
using EkfState = EkfVelocity::state_mtx;

autodiff::dual velocity_state_x(const EkfStateInp& x)
{
    const autodiff::dual& vx = x(0);
    const autodiff::dual& vy = x(1);
    const autodiff::dual& ax = x(2);
    const autodiff::dual& yaw = x(4);
    return vx + kTimeStep * (ax + vy * yaw);
}

autodiff::dual velocity_state_y(const EkfStateInp& x)
{
    const autodiff::dual& vx = x(0);
    const autodiff::dual& vy = x(1);
    const autodiff::dual& ay = x(3);
    const autodiff::dual& yaw = x(4);
    return vy + kTimeStep * (ay + vx * yaw);
}

autodiff::dual velocity_meas_x(const EkfState& x)
{
    return x(0);
}

autodiff::dual velocity_meas_y(const EkfState& x)
{
    return x(1);
}

auto velocity_model_step(const EkfV2& x, const EkfU3& u) -> EkfV2
{
    EkfV2 next;
    next(0) = x(0) + kTimeStep * (u(0) + x(1) * u(2));
    next(1) = x(1) + kTimeStep * (u(1) + x(0) * u(2));
    return next;
}

auto velocity_model_F(const EkfU3& u) -> EkfN2
{
    EkfN2 F;
    F << 1.0f, kTimeStep * u(2),
         kTimeStep * u(2), 1.0f;
    return F;
}

auto create_velocity_state_functions() -> std::array<EkfVelocity::StateFunction, 2>
{
    return {{
        velocity_state_x,
        velocity_state_y,
    }};
}

auto create_velocity_measurement_functions() -> std::array<EkfVelocity::MeasurementFunction, 2>
{
    return {{
        velocity_meas_x,
        velocity_meas_y,
    }};
}
} // namespace

/**
 * Tests that one KF update exactly matches closed-form linear KF equations.
 *
 * How:
 * - Builds a 2-state, 1-measurement linear system and computes one manual
 *   predict/update step.
 * - Runs one `kf::estimated_states(...)` step with the same data.
 * - Compares posterior state and covariance element-wise.
 */
TEST(KalmanFilterTest, OneStepMatchesAnalyticalReference)
{
    KfN2 F;
    F << 1.0f, kTimeStep,
         0.0f, 1.0f;

    KfV2 B;
    B << 0.5f * kTimeStep * kTimeStep,
         kTimeStep;

    KfN2 Q;
    Q << 1e-4f, 0.0f,
         0.0f, 1e-3f;

    KfH H;
    H << 0.0f, 1.0f;

    KfM1 R;
    R << 0.04f;

    KfV2 x0;
    x0 << 0.3f, 1.5f;

    KfN2 P0;
    P0 << 0.8f, 0.1f,
          0.1f, 0.6f;

    KfVelocity filter(F, B, Q, H, R, x0, P0);

    KfU1 u;
    u << 2.0f;

    KfM1 z;
    z << 1.8f;

    const KfV2 x_pred = F * x0 + B * u(0);
    const KfN2 P_pred = F * P0 * F.transpose() + Q;
    const KfM1 S = H * P_pred * H.transpose() + R;
    const Eigen::Matrix<float, 2, 1> K = P_pred * H.transpose() * S.llt().solve(KfM1::Identity());
    const KfM1 y = z - H * x_pred;
    const KfV2 x_expected = x_pred + K * y(0);
    const KfN2 P_expected = (KfN2::Identity() - K * H) * P_pred;

    const KfV2 x_actual = filter.estimated_states(u, z);
    const KfN2 P_actual = filter.covariance();

    for (int i = 0; i < 2; ++i)
    {
        EXPECT_NEAR(x_expected(i), x_actual(i), 1e-6f);
    }

    for (int r = 0; r < 2; ++r)
    {
        for (int c = 0; c < 2; ++c)
        {
            EXPECT_NEAR(P_expected(r, c), P_actual(r, c), 1e-6f);
        }
    }
}

/**
 * Makeshift SIL loop test for KF long-run stability under noisy measurements.
 *
 * How:
 * - Simulates a linear plant for 1200 steps with a varying acceleration input.
 * - Feeds noisy velocity measurements to the KF each step.
 * - Checks finite outputs, covariance symmetry, and bounded RMSE.
 */
TEST(KalmanFilterTest, SilLoopRemainsStableAndTracksVelocity)
{
    KfN2 F;
    F << 1.0f, kTimeStep,
         0.0f, 1.0f;

    KfV2 B;
    B << 0.5f * kTimeStep * kTimeStep,
         kTimeStep;

    KfN2 Q;
    Q << 5e-4f, 0.0f,
         0.0f, 2e-3f;

    KfH H;
    H << 0.0f, 1.0f;

    KfM1 R;
    R << 0.06f;

    KfV2 x0;
    x0 << 0.0f, 0.0f;

    KfN2 P0;
    P0 << 1.0f, 0.0f,
          0.0f, 1.0f;

    KfVelocity filter(F, B, Q, H, R, x0, P0);

    std::mt19937 rng(7);
    std::normal_distribution<float> meas_noise(0.0f, 0.08f);

    KfV2 x_true = x0;
    float rmse_acc = 0.0f;
    int rmse_count = 0;

    constexpr int kSteps = 1200;
    for (int step = 0; step < kSteps; ++step)
    {
        const float t = static_cast<float>(step) * kTimeStep;

        KfU1 u;
        u << 1.5f + 0.2f * std::sin(0.15f * t);

        x_true = F * x_true + B * u(0);

        KfM1 z;
        z << x_true(1) + meas_noise(rng);

        const KfV2 x_est = filter.estimated_states(u, z);
        const KfN2 P = filter.covariance();

        EXPECT_TRUE(x_est.allFinite());
        EXPECT_TRUE(P.allFinite());
        EXPECT_NEAR(P(0, 1), P(1, 0), 1e-4f);
        EXPECT_GT(P(0, 0), -1e-6f);
        EXPECT_GT(P(1, 1), -1e-6f);

        if (step > 200)
        {
            const float err = x_est(1) - x_true(1);
            rmse_acc += err * err;
            ++rmse_count;
        }
    }

    const float velocity_rmse = std::sqrt(rmse_acc / static_cast<float>(rmse_count));
    EXPECT_LT(velocity_rmse, 0.2f);
}

#if !defined(NDEBUG)
/**
 * Edge-case constructor validation test for KF covariance constraints.
 *
 * How:
 * - Provides a non-PSD P0 and verifies constructor aborts with a descriptive
 *   assertion message.
 */
TEST(KalmanFilterTest, ConstructorRejectsNonPsdP0)
{
    KfN2 F;
    F << 1.0f, kTimeStep,
         0.0f, 1.0f;

    KfV2 B;
    B << 0.5f * kTimeStep * kTimeStep,
         kTimeStep;

    KfN2 Q;
    Q << 1e-4f, 0.0f,
         0.0f, 1e-3f;

    KfH H;
    H << 0.0f, 1.0f;

    KfM1 R;
    R << 0.05f;

    KfV2 x0;
    x0 << 0.0f, 0.0f;

    KfN2 bad_P0;
    bad_P0 << 1.0f, 2.0f,
              2.0f, 1.0f;

    EXPECT_DEATH(
        {
            KfVelocity invalid_filter(F, B, Q, H, R, x0, bad_P0);
            (void)invalid_filter;
        },
        "P0 must be positive semidefinite");
}
#endif

/**
 * Tests EKF one-step result against a hand-computed linearized reference.
 *
 * How:
 * - Uses the same nonlinear model as the EKF test fixture functions.
 * - Computes predict/update manually using analytic Jacobian F and H = I.
 * - Verifies EKF output matches the reference state/covariance.
 */
TEST(ExtendedKalmanFilterTest, OneStepMatchesLinearizedReference)
{
    const auto f = create_velocity_state_functions();
    const auto h = create_velocity_measurement_functions();

    EkfV2 x0;
    x0 << 1.0f, 2.0f;

    EkfN2 P0;
    P0 << 1.0f, 0.2f,
          0.2f, 1.5f;

    EkfN2 Q;
    Q << 2e-4f, 0.0f,
         0.0f, 2e-4f;

    EkfN2 R;
    R << 0.04f, 0.0f,
         0.0f, 0.09f;

    EkfVelocity filter(f, h, Q, R, x0, P0);

    EkfU3 u;
    u << 1.0f, 2.0f, 3.0f;

    EkfV2 z;
    z << 1.5f, 2.5f;

    const EkfV2 x_pred = velocity_model_step(x0, u);
    const EkfN2 F = velocity_model_F(u);
    const EkfN2 P_pred = F * P0 * F.transpose() + Q;
    const EkfN2 S = P_pred + R;
    const EkfN2 K = P_pred * S.llt().solve(EkfN2::Identity());
    const EkfV2 y = z - x_pred;
    const EkfV2 x_expected = x_pred + K * y;
    const EkfN2 P_expected = (EkfN2::Identity() - K) * P_pred;

    const EkfV2 x_actual = filter.estimated_states(u, z);
    const EkfN2 P_actual = filter.covariance();

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

/**
 * Edge-case EKF behavior with near-zero measurement noise.
 *
 * How:
 * - Uses very small but positive-definite R to satisfy LLT assumptions.
 * - Verifies the update strongly snaps state toward the measurement and shrinks
 *   posterior covariance.
 */
TEST(ExtendedKalmanFilterTest, VeryLowMeasurementNoiseSnapsTowardMeasurement)
{
    const auto f = create_velocity_state_functions();
    const auto h = create_velocity_measurement_functions();

    EkfV2 x0;
    x0 << 1.0f, 2.0f;

    EkfN2 P0;
    P0 << 0.9f, 0.0f,
          0.0f, 0.7f;

    const EkfN2 Q = EkfN2::Zero();

    EkfN2 R;
    R << 1e-8f, 0.0f,
         0.0f, 1e-8f;

    EkfVelocity filter(f, h, Q, R, x0, P0);

    EkfU3 u;
    u << 1.0f, 2.0f, 3.0f;

    EkfV2 z;
    z << 1.5f, 2.5f;

    const EkfV2 x_actual = filter.estimated_states(u, z);
    const EkfN2 P_actual = filter.covariance();

    for (int i = 0; i < 2; ++i)
    {
        EXPECT_NEAR(z(i), x_actual(i), 2e-3f);
    }

    EXPECT_LT(P_actual(0, 0), 2e-4f);
    EXPECT_LT(P_actual(1, 1), 2e-4f);
}

/**
 * Makeshift SIL loop test for EKF long-run stability and tracking.
 *
 * How:
 * - Simulates nonlinear velocity dynamics with time-varying inputs for 1500 steps.
 * - Injects measurement noise and runs EKF predict/update each step.
 * - Checks finite state/covariance, covariance symmetry, and bounded tracking RMSE.
 */
TEST(ExtendedKalmanFilterTest, SilLoopRemainsStableAndTracksState)
{
    const auto f = create_velocity_state_functions();
    const auto h = create_velocity_measurement_functions();

    EkfV2 x0;
    x0 << 0.0f, 0.0f;

    EkfN2 P0;
    P0 << 1.0f, 0.0f,
          0.0f, 1.0f;

    EkfN2 Q;
    Q << 8e-4f, 0.0f,
         0.0f, 8e-4f;

    EkfN2 R;
    R << 0.04f, 0.0f,
         0.0f, 0.04f;

    EkfVelocity filter(f, h, Q, R, x0, P0);

    std::mt19937 rng(21);
    std::normal_distribution<float> meas_noise(0.0f, 0.12f);

    EkfV2 x_true = x0;
    float rmse_acc = 0.0f;
    int rmse_count = 0;

    constexpr int kSteps = 1500;
    for (int step = 0; step < kSteps; ++step)
    {
        const float t = static_cast<float>(step) * kTimeStep;

        EkfU3 u;
        u << 1.2f + 0.25f * std::sin(0.7f * t),
             0.9f + 0.20f * std::cos(0.4f * t),
             0.3f + 0.05f * std::sin(0.15f * t);

        x_true = velocity_model_step(x_true, u);

        EkfV2 z;
        z << x_true(0) + meas_noise(rng),
             x_true(1) + meas_noise(rng);

        const EkfV2 x_est = filter.estimated_states(u, z);
        const EkfN2 P = filter.covariance();

        EXPECT_TRUE(x_est.allFinite());
        EXPECT_TRUE(P.allFinite());
        EXPECT_NEAR(P(0, 1), P(1, 0), 2e-3f);
        EXPECT_GT(P(0, 0), -1e-6f);
        EXPECT_GT(P(1, 1), -1e-6f);

        if (step > 300)
        {
            rmse_acc += (x_est - x_true).squaredNorm();
            ++rmse_count;
        }
    }

    const float rmse = std::sqrt(rmse_acc / static_cast<float>(2 * rmse_count));
    EXPECT_LT(rmse, 0.25f);
}

#if !defined(NDEBUG)
/**
 * Edge-case constructor validation test for EKF measurement covariance.
 *
 * How:
 * - Passes a non-PD measurement covariance (R = 0) and verifies constructor
 *   aborts with a descriptive assertion message.
 */
TEST(ExtendedKalmanFilterTest, ConstructorRejectsNonPdR)
{
    const auto f = create_velocity_state_functions();
    const auto h = create_velocity_measurement_functions();

    EkfV2 x0;
    x0 << 0.0f, 0.0f;

    EkfN2 P0;
    P0 << 1.0f, 0.0f,
          0.0f, 1.0f;

    const EkfN2 Q = EkfN2::Zero();
    const EkfN2 bad_R = EkfN2::Zero();

    EXPECT_DEATH(
        {
            EkfVelocity invalid_filter(f, h, Q, bad_R, x0, P0);
            (void)invalid_filter;
        },
        "R must be positive definite");
}
#endif
