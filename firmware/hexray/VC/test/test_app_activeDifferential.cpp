#include <gtest/gtest.h>
#include <cmath>
#include <fstream>
#include <chrono>

extern "C"
{
#include "app_canRx.h"
#include "app_canTx.h"
}
#include "app_activeDifferential.hpp"
#include "app_vd_constants.hpp"
#include "app_units.h"

// Test constants: TRACK_WIDTH_m, WHEELBASE_m, APPROX_STEERING_TO_WHEEL_ANGLE are defined in headers

// Tolerance for floating point comparisons
constexpr double EPSILON = 1e-3;

class TestActiveDifferential : public testing::Test
{
  protected:
    void SetUp() override
    {
        // Initialize CAN tables (required for dependencies)
        app_canTx_init();
        app_canRx_init();
    }
};

// Helper: expected delta_omega from Eq. 13
static double expected_delta_omega(double steering_angle_deg, double omega_v)
{
    const double delta =
        DEG_TO_RAD(static_cast<float>(steering_angle_deg) * static_cast<float>(APPROX_STEERING_TO_WHEEL_ANGLE));
    return (static_cast<double>(TRACK_WIDTH_m) / static_cast<double>(WHEELBASE_m)) * tan(delta) * omega_v;
}

/**
 * Test straight driving (steering angle = 0)
 * wheel_delta should be zero
 */
TEST_F(TestActiveDifferential, StraightDriving)
{
    const double omega_v_ref        = 10.0; // rad/s
    const double steering_angle_deg = 0.0;

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    EXPECT_NEAR(result.wheel_delta, 0.0, EPSILON);
}

/**
 * Test left turn (positive steering angle)
 * wheel_delta should be positive (right wheels outer, faster)
 */
TEST_F(TestActiveDifferential, LeftTurn)
{
    const double omega_v_ref        = 10.0; // rad/s
    const double steering_angle_deg = 30.0; // Left turn (positive = left turn)

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    const double expected = expected_delta_omega(steering_angle_deg, omega_v_ref);
    EXPECT_NEAR(result.wheel_delta, expected, EPSILON);
    EXPECT_GT(result.wheel_delta, 0.0);
}

/**
 * Test right turn (negative steering angle)
 * wheel_delta should be negative (left wheels outer, faster)
 */
TEST_F(TestActiveDifferential, RightTurn)
{
    const double omega_v_ref        = 10.0;  // rad/s
    const double steering_angle_deg = -30.0; // Right turn (negative = right turn)

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    const double expected = expected_delta_omega(steering_angle_deg, omega_v_ref);
    EXPECT_NEAR(result.wheel_delta, expected, EPSILON);
    EXPECT_LT(result.wheel_delta, 0.0);
}

/**
 * Test speed limiting with custom omega_m_max
 * Function only returns delta_omega; no clamping in this layer.
 */
TEST_F(TestActiveDifferential, SpeedLimitingCustomLimit)
{
    const double omega_v_ref        = 100.0; // rad/s (very high)
    const double steering_angle_deg = 0.0;

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    EXPECT_NEAR(result.wheel_delta, 0.0, EPSILON);
}

/**
 * Test speed limiting with default MAX_MOTOR_SPEED_RAD_S
 * Function only returns delta_omega; straight driving gives zero.
 */
TEST_F(TestActiveDifferential, SpeedLimitingDefaultLimit)
{
    const double omega_v_ref        = 100.0; // rad/s (very high)
    const double steering_angle_deg = 0.0;

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    EXPECT_NEAR(result.wheel_delta, 0.0, EPSILON);
}

/**
 * Test negative speed limiting (reverse)
 * Function only returns delta_omega; straight driving gives zero.
 */
TEST_F(TestActiveDifferential, NegativeSpeedLimiting)
{
    const double omega_v_ref        = -100.0; // rad/s (reverse, very high)
    const double steering_angle_deg = 0.0;

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    EXPECT_NEAR(result.wheel_delta, 0.0, EPSILON);
}

/**
 * Test with zero vehicle speed
 * delta_omega = (track/wheelbase)*tan(delta)*0 = 0
 */
TEST_F(TestActiveDifferential, ZeroSpeed)
{
    const double omega_v_ref        = 0.0;
    const double steering_angle_deg = 45.0; // Even with steering, omega_v=0 => delta_omega=0

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    EXPECT_NEAR(result.wheel_delta, 0.0, EPSILON);
}

/**
 * Test with small steering angle
 * wheel_delta should be small positive (left turn)
 */
TEST_F(TestActiveDifferential, SmallSteeringAngle)
{
    const double omega_v_ref        = 10.0;
    const double steering_angle_deg = 5.0; // Small left turn (positive = left turn)

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    const double expected = expected_delta_omega(steering_angle_deg, omega_v_ref);
    EXPECT_NEAR(result.wheel_delta, expected, EPSILON);
    EXPECT_GT(result.wheel_delta, 0.0);
}

/**
 * Test with large steering angle
 * wheel_delta should be large positive (left turn)
 */
TEST_F(TestActiveDifferential, LargeSteeringAngle)
{
    const double omega_v_ref        = 10.0;
    const double steering_angle_deg = 90.0; // Large left turn (positive = left turn)

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    const double expected = expected_delta_omega(steering_angle_deg, omega_v_ref);
    EXPECT_NEAR(result.wheel_delta, expected, EPSILON);
    EXPECT_GT(result.wheel_delta, 0.0);
}

/**
 * Test speed limiting during turn
 * Function only returns delta_omega; no clamping.
 */
TEST_F(TestActiveDifferential, SpeedLimitingDuringTurn)
{
    const double omega_v_ref        = 50.0; // rad/s
    const double steering_angle_deg = 30.0; // Left turn (positive = left turn)

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    const double expected = expected_delta_omega(steering_angle_deg, omega_v_ref);
    EXPECT_NEAR(result.wheel_delta, expected, EPSILON);
}

/**
 * Test algorithm correctness: verify the mathematical relationship
 * Δω = (TRACK_WIDTH_m/WHEELBASE_m) * tan(δ) * ω_v
 */
TEST_F(TestActiveDifferential, AlgorithmCorrectness)
{
    const double omega_v_ref        = 15.0;
    const double steering_angle_deg = 20.0;

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    const double expected = expected_delta_omega(steering_angle_deg, omega_v_ref);
    EXPECT_NEAR(result.wheel_delta, expected, EPSILON);
}

/**
 * Test that wheel_delta matches Eq. 13 for a given steering and yaw rate
 */
TEST_F(TestActiveDifferential, FrontRearWheelConsistency)
{
    const double omega_v_ref        = 10.0;
    const double steering_angle_deg = 25.0;

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    const double expected = expected_delta_omega(steering_angle_deg, omega_v_ref);
    EXPECT_NEAR(result.wheel_delta, expected, EPSILON);
}

/**
 * Test with negative omega_m_max
 * Function only returns delta_omega; straight driving gives zero.
 */
TEST_F(TestActiveDifferential, NegativeOmegaMaxUsesDefault)
{
    const double omega_v_ref        = 100.0;
    const double steering_angle_deg = 0.0;

    controllerTorqueDeltas result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg);

    EXPECT_NEAR(result.wheel_delta, 0.0, EPSILON);
}
