#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"

#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "util_units.hpp"

#include "app_sbgEllipse.hpp"
#include "app_canUtils.hpp"
#include "torque_vectoring/estimators/velocity_estimator.hpp"
#include "torque_vectoring/datatypes/datatypes_vd_constants.hpp"

using namespace app::tv::estimators::velocity_estimator;
using namespace app::tv::estimators::velocity_estimator::detail;
using namespace app::tv::datatypes::vd_constants;

static constexpr float kSlipThres = 0.0117f;
static constexpr float kVxMin     = 0.5f;
static constexpr float kTol       = 1e-5f;

namespace app::tv::estimators::velocity_estimator
{
extern ProcessNoiseCov                 Q;
extern VelocityEstimator::PredictStep  system_model;
extern VelocityEstimator::UpdateSteps  update_steps;
extern VelocityEstimator               velocity_estimator;
} // namespace app::tv::estimators::velocity_estimator

namespace io::sbgEllipse
{
void setEkfSolutionMode(uint32_t ekf_sol_mode_);
} // namespace io::sbgEllipse

namespace
{
VelocityEstimator makeEstimator(float vx0 = 0.0f, float vy0 = 0.0f)
{
    Velocity x0 = Velocity::Zero();
    x0 << vx0, vy0;
    return VelocityEstimator(system_model, Q, update_steps, x0, ProcessNoiseCov::Zero());
}

void resetEstimator(float vx0 = 0.0f, float vy0 = 0.0f)
{
    velocity_estimator = makeEstimator(vx0, vy0);
}

void setGpsMode(const app::can_utils::VcEkfStatus mode)
{
    io::sbgEllipse::setEkfSolutionMode(static_cast<uint32_t>(mode));
    app::sbgEllipse::broadcast();
}

struct VehicleBodyMotion
{
    float vx_mps      = 0.0f;
    float vy_mps      = 0.0f;
    float yaw_rads    = 0.0f;
    float ax_mps2     = 0.0f;
    float ay_mps2     = 0.0f;
    float ang_fr_rad  = 0.0f;
    float ang_fl_rad  = 0.0f;
    float rr_ws_mps   = 0.0f;
    float rl_ws_mps   = 0.0f;
    float fr_ws_mps   = 0.0f;
    float fl_ws_mps   = 0.0f;
};

VehicleBodyMotion makeSteadyStateBodyMotion(float vx_mps, float vy_mps, float yaw_rads)
{
    VehicleBodyMotion motion{};
    motion.vx_mps   = vx_mps;
    motion.vy_mps   = vy_mps;
    motion.yaw_rads = yaw_rads;

    // Keep the body-frame velocity constant under the estimator process model:
    // vx_dot = ax - vy * r, vy_dot = ay + vx * r.
    motion.ax_mps2 = vy_mps * yaw_rads;
    motion.ay_mps2 = -vx_mps * yaw_rads;

    const float rr_vx_body   = vx_mps - yaw_rads * HALF_TRACK_WIDTH_m;
    const float rl_vx_body   = vx_mps + yaw_rads * HALF_TRACK_WIDTH_m;
    const float fr_vx_body   = vx_mps - yaw_rads * HALF_TRACK_WIDTH_m;
    const float fl_vx_body   = vx_mps + yaw_rads * HALF_TRACK_WIDTH_m;
    const float front_vy_term = yaw_rads * DIST_FRONT_AXLE_CG_m - vy_mps;

    motion.rr_ws_mps  = rr_vx_body;
    motion.rl_ws_mps  = rl_vx_body;
    motion.ang_fr_rad = std::atan2(front_vy_term, fr_vx_body);
    motion.ang_fl_rad = std::atan2(front_vy_term, fl_vx_body);
    motion.fr_ws_mps  = std::hypot(fr_vx_body, front_vy_term);
    motion.fl_ws_mps  = std::hypot(fl_vx_body, front_vy_term);

    return motion;
}

VelocityEstimatorInputs makeEstimatorInputs(const VehicleBodyMotion &motion)
{
    VelocityEstimatorInputs inputs{};
    inputs.control.ax_mps2   = motion.ax_mps2;
    inputs.control.ay_mps2   = motion.ay_mps2;
    inputs.control.r_rads    = motion.yaw_rads;
    inputs.wheels.rr_rpm     = static_cast<float>(MOTOR_KMH_TO_RPM(MPS_TO_KMH(motion.rr_ws_mps)));
    inputs.wheels.rl_rpm     = static_cast<float>(MOTOR_KMH_TO_RPM(MPS_TO_KMH(motion.rl_ws_mps)));
    inputs.wheels.fr_rpm     = static_cast<float>(MOTOR_KMH_TO_RPM(MPS_TO_KMH(motion.fr_ws_mps)));
    inputs.wheels.fl_rpm     = static_cast<float>(MOTOR_KMH_TO_RPM(MPS_TO_KMH(motion.fl_ws_mps)));
    inputs.wheels.ang_fr_rad = motion.ang_fr_rad;
    inputs.wheels.ang_fl_rad = motion.ang_fl_rad;
    inputs.gps.body_vx_mps   = motion.vx_mps;
    inputs.gps.body_vy_mps   = motion.vy_mps;
    return inputs;
}

Velocity runEstimatorLoop(
    const VelocityEstimatorInputs       &inputs,
    const app::can_utils::VcEkfStatus    gps_mode,
    uint32_t                             steps)
{
    setGpsMode(gps_mode);

    Velocity estimate = Velocity::Zero();
    for (uint32_t i = 0; i < steps; i++)
    {
        estimate = estimate_body_velocity(inputs);
    }

    return estimate;
}

} // namespace

class VCVelocityEstimatorTest : public VCBaseTest
{
};

// ==================== computeSlipRatio ====================

TEST(VCVelocityEstimatorTest, ZeroSlipWhenWheelMatchesState)
{
    EXPECT_NEAR(computeSlipRatio(2.0f, 2.0f), 0.0f, kTol);
}

TEST(VCVelocityEstimatorTest, PositiveSlipWhenWheelFaster)
{
    const float slip = computeSlipRatio(3.0f, 2.0f);
    EXPECT_GT(slip, 0.0f);
    EXPECT_NEAR(slip, 1.0f / 2.0f, kTol);
}

TEST(VCVelocityEstimatorTest, NegativeSlipWhenWheelSlower)
{
    const float slip = computeSlipRatio(1.0f, 2.0f);
    EXPECT_LT(slip, 0.0f);
}

TEST(VCVelocityEstimatorTest, ClampsDenominatorToVxMin)
{
    // vx_state = 0 should clamp denominator to VX_MIN_MPS
    const float slip = computeSlipRatio(1.0f, 0.0f);
    EXPECT_NEAR(slip, 1.0f / kVxMin, kTol);
}

TEST(VCVelocityEstimatorTest, ClampsDenominatorWhenStateBelowMin)
{
    // vx_state = 0.1 < VX_MIN_MPS, denominator should be VX_MIN_MPS
    const float slip = computeSlipRatio(1.0f, 0.1f);
    EXPECT_NEAR(slip, (1.0f - 0.1f) / kVxMin, kTol);
}

// ==================== computeWheelVx ====================

TEST(VCVelocityEstimatorTest, StraightLineNoYaw)
{
    // With zero yaw, vx_wheel = ws_mps * cos(delta)
    Eigen::Matrix<float, 4, 1> ws;
    ws << 5.0f, 5.0f, 5.0f, 5.0f;

    const float cos_fr = 1.0f; // delta = 0
    const float cos_fl = 1.0f;
    const float yaw    = 0.0f;

    const auto vx = computeWheelVx(ws, cos_fr, cos_fl, yaw);

    // All wheels should report same speed with zero steering and yaw
    EXPECT_NEAR(vx[0], 5.0f, kTol); // rr
    EXPECT_NEAR(vx[1], 5.0f, kTol); // rl
    EXPECT_NEAR(vx[2], 5.0f, kTol); // fr
    EXPECT_NEAR(vx[3], 5.0f, kTol); // fl
}

TEST(VCVelocityEstimatorTest, YawCouplingAddsCorrectly)
{
    Eigen::Matrix<float, 4, 1> ws;
    ws << 5.0f, 5.0f, 5.0f, 5.0f;

    const float yaw = 1.0f; // rad/s

    const auto vx = computeWheelVx(ws, 1.0f, 1.0f, yaw);

    // rr: ws + yaw * half_track, rl: ws - yaw * half_track
    EXPECT_NEAR(vx[0], 5.0f + yaw * HALF_TRACK_WIDTH_m, kTol);
    EXPECT_NEAR(vx[1], 5.0f - yaw * HALF_TRACK_WIDTH_m, kTol);
    EXPECT_NEAR(vx[2], 5.0f + yaw * HALF_TRACK_WIDTH_m, kTol); // cos = 1
    EXPECT_NEAR(vx[3], 5.0f - yaw * HALF_TRACK_WIDTH_m, kTol); // cos = 1
}

TEST(VCVelocityEstimatorTest, SteeringAngleScalesFrontWheels)
{
    Eigen::Matrix<float, 4, 1> ws;
    ws << 5.0f, 5.0f, 5.0f, 5.0f;

    const float delta  = 0.2f; // rad
    const float cos_fr = std::cosf(delta);
    const float cos_fl = std::cosf(delta);

    const auto vx = computeWheelVx(ws, cos_fr, cos_fl, 0.0f);

    // Rear wheels unaffected by steering
    EXPECT_NEAR(vx[0], 5.0f, kTol);
    EXPECT_NEAR(vx[1], 5.0f, kTol);
    // Front wheels scaled by cos(delta)
    EXPECT_NEAR(vx[2], 5.0f * cos_fr, kTol);
    EXPECT_NEAR(vx[3], 5.0f * cos_fl, kTol);
}

// ==================== computeWheelVy ====================

TEST(VCVelocityEstimatorTest, ZeroYawZeroVy)
{
    Eigen::Matrix<float, 4, 1> ws;
    ws << 5.0f, 5.0f, 5.0f, 5.0f;

    const auto vy = computeWheelVy(ws, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++)
        EXPECT_NEAR(vy[i], 0.0f, kTol);
}

TEST(VCVelocityEstimatorTest, YawCouplingCorrect)
{
    Eigen::Matrix<float, 4, 1> ws;
    ws << 5.0f, 5.0f, 5.0f, 5.0f;

    const float yaw = 1.0f;
    const auto  vy  = computeWheelVy(ws, 0.0f, 0.0f, yaw); // sin_delta = 0

    EXPECT_NEAR(vy[0], -yaw * DIST_REAR_AXLE_CG_m, kTol); // rr
    EXPECT_NEAR(vy[1], -yaw * DIST_REAR_AXLE_CG_m, kTol); // rl
    EXPECT_NEAR(vy[2], yaw * DIST_FRONT_AXLE_CG_m, kTol); // fr (sin=0 so no ws contribution)
    EXPECT_NEAR(vy[3], yaw * DIST_FRONT_AXLE_CG_m, kTol); // fl
}

TEST(VCVelocityEstimatorTest, SteeringAngleSubtractsFrontWheelVy)
{
    Eigen::Matrix<float, 4, 1> ws;
    ws << 0.0f, 0.0f, 5.0f, 5.0f;

    const float delta  = 0.2f;
    const float sin_fr = std::sinf(delta);
    const float sin_fl = std::sinf(delta);

    const auto vy = computeWheelVy(ws, sin_fr, sin_fl, 0.0f);

    EXPECT_NEAR(vy[0], 0.0f, kTol);
    EXPECT_NEAR(vy[1], 0.0f, kTol);
    EXPECT_NEAR(vy[2], -5.0f * sin_fr, kTol);
    EXPECT_NEAR(vy[3], -5.0f * sin_fl, kTol);
}

// ==================== averageValidWheels ====================

TEST(VCVelocityEstimatorTest, AllValidWheelsAveraged)
{
    Eigen::Matrix<float, 4, 1> vx_wheels;
    vx_wheels << 2.0f, 2.0f, 2.0f, 2.0f;

    Eigen::Matrix<float, 4, 1> vy_wheels;
    vy_wheels << 0.1f, 0.1f, 0.1f, 0.1f;

    const auto result = averageValidWheels(vx_wheels, vy_wheels, 2.0f);

    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->operator()(0), 2.0f, kTol);
    EXPECT_NEAR(result->operator()(1), 0.1f, kTol);
}

TEST(VCVelocityEstimatorTest, ReturnsNulloptWhenAllWheelsSlipping)
{
    Eigen::Matrix<float, 4, 1> vx_wheels;
    vx_wheels << 10.0f, 10.0f, 10.0f, 10.0f; // all far from vx_state

    Eigen::Matrix<float, 4, 1> vy_wheels;
    vy_wheels << 0.0f, 0.0f, 0.0f, 0.0f;

    const auto result = averageValidWheels(vx_wheels, vy_wheels, 2.0f);

    EXPECT_FALSE(result.has_value());
}

TEST(VCVelocityEstimatorTest, ExcludesSlippingWheelFromAverage)
{
    // Three good wheels at 2.0, one slipping at 10.0
    Eigen::Matrix<float, 4, 1> vx_wheels;
    vx_wheels << 2.0f, 2.0f, 2.0f, 10.0f;

    Eigen::Matrix<float, 4, 1> vy_wheels;
    vy_wheels << 0.0f, 0.0f, 0.0f, 0.0f;

    const auto result = averageValidWheels(vx_wheels, vy_wheels, 2.0f);

    ASSERT_TRUE(result.has_value());
    // Should average only the three valid wheels
    EXPECT_NEAR(result->operator()(0), 2.0f, kTol);
}

TEST(VCVelocityEstimatorTest, ZeroVxStateUsesVxMinAsDenominator)
{
    // With vx_state = 0, denominator clamps to VX_MIN_MPS
    // wheels at 0 + small offset: slip = offset / VX_MIN_MPS
    // to stay under threshold: offset < SLIP_THRES * VX_MIN_MPS
    const float safe_offset = kSlipThres * kVxMin * 0.5f;

    Eigen::Matrix<float, 4, 1> vx_wheels;
    vx_wheels << safe_offset, safe_offset, safe_offset, safe_offset;

    Eigen::Matrix<float, 4, 1> vy_wheels;
    vy_wheels << 0.0f, 0.0f, 0.0f, 0.0f;

    const auto result = averageValidWheels(vx_wheels, vy_wheels, 0.0f);

    ASSERT_TRUE(result.has_value());
}

TEST(VCVelocityEstimatorTest, MixedValidInvalidWheelsAveragesCorrectly)
{
    // Two valid at 3.0, two slipping at 10.0
    Eigen::Matrix<float, 4, 1> vx_wheels;
    vx_wheels << 3.0f, 3.0f, 10.0f, 10.0f;

    Eigen::Matrix<float, 4, 1> vy_wheels;
    vy_wheels << 1.0f, 1.0f, 5.0f, 5.0f;

    const auto result = averageValidWheels(vx_wheels, vy_wheels, 3.0f);

    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->operator()(0), 3.0f, kTol);
    EXPECT_NEAR(result->operator()(1), 1.0f, kTol);
}

TEST(VCVelocityEstimatorTest, ConvergesToStraightLineBodyVelocityFromWheelSpeeds)
{
    const auto motion = makeSteadyStateBodyMotion(8.0f, 0.0f, 0.0f);
    const auto inputs = makeEstimatorInputs(motion);
    resetEstimator(motion.vx_mps + 0.05f, 0.02f);

    const auto estimate = runEstimatorLoop(inputs, app::can_utils::VcEkfStatus::UNINITIALIZED, 150);

    EXPECT_NEAR(estimate(0), motion.vx_mps, 0.02f);
    EXPECT_NEAR(estimate(1), motion.vy_mps, 0.02f);
}

TEST(VCVelocityEstimatorTest, ConvergesToSteadyStateCorneringMotionFromWheelAndGps)
{
    resetEstimator();

    const auto motion = makeSteadyStateBodyMotion(10.0f, -0.35f, 0.55f);
    const auto inputs = makeEstimatorInputs(motion);

    ASSERT_GT(motion.ang_fr_rad, 0.0f);
    ASSERT_GT(motion.ang_fl_rad, 0.0f);

    const auto estimate = runEstimatorLoop(inputs, app::can_utils::VcEkfStatus::POSITION, 200);

    EXPECT_NEAR(estimate(0), motion.vx_mps, 0.02f);
    EXPECT_NEAR(estimate(1), motion.vy_mps, 0.02f);
}

TEST(VCVelocityEstimatorTest, GpsMeasurementIsIgnoredOutsidePositionMode)
{
    const auto motion = makeSteadyStateBodyMotion(6.0f, -0.25f, 0.45f);
    auto       inputs = makeEstimatorInputs(motion);
    resetEstimator(motion.vx_mps + 0.04f, motion.vy_mps - 0.03f);
    inputs.gps.body_vx_mps = 20.0f;
    inputs.gps.body_vy_mps = 3.0f;

    const auto estimate = runEstimatorLoop(inputs, app::can_utils::VcEkfStatus::UNINITIALIZED, 100);

    EXPECT_NEAR(estimate(0), motion.vx_mps, 0.02f);
    EXPECT_NEAR(estimate(1), motion.vy_mps, 0.04f);
}

TEST(VCVelocityEstimatorTest, RejectsSingleSlippingWheelAndStillConverges)
{
    resetEstimator();

    const auto motion = makeSteadyStateBodyMotion(9.0f, -0.2f, 0.4f);
    auto       inputs = makeEstimatorInputs(motion);

    // Inject a large front-left slip so that wheel should be excluded.
    inputs.wheels.fl_rpm = static_cast<float>(MOTOR_KMH_TO_RPM(MPS_TO_KMH(motion.fl_ws_mps * 1.3f)));

    const auto estimate = runEstimatorLoop(inputs, app::can_utils::VcEkfStatus::POSITION, 300);

    EXPECT_NEAR(estimate(0), motion.vx_mps, 0.05f);
    EXPECT_NEAR(estimate(1), motion.vy_mps, 0.06f);
}
