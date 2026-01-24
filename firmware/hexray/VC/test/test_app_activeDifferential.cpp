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

// #region agent log
// Debug: Verify test file is being compiled and executed
// This simple test should always register if the file is compiled
TEST(TestFileCompilation, VerifyTestFileIsCompiled)
{
    EXPECT_TRUE(true);
    std::ofstream log_file(
        "c:\\Users\\hitar\\OneDrive\\Desktop\\UBC FE\\Consolidated-Firmware\\.cursor\\debug.log", std::ios::app);
    if (log_file.is_open())
    {
        auto now =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                .count();
        log_file << "{\"id\":\"log_test_compiled\",\"timestamp\":" << now
                 << ",\"location\":\"test_app_activeDifferential.cpp:17\",\"message\":\"Test file "
                    "executed\",\"data\":{\"testName\":\"VerifyTestFileIsCompiled\"},\"sessionId\":\"debug-session\","
                    "\"runId\":\"run1\",\"hypothesisId\":\"A\"}\n";
        log_file.close();
    }
}
// #endregion

// Test constants - use macros from headers directly
// TRACK_WIDTH_m, WHEELBASE_m, GEAR_RATIO, APPROX_STEERING_TO_WHEEL_ANGLE are defined in headers
constexpr double MAX_MOTOR_SPEED_RPM   = 6000.0;
constexpr double MAX_MOTOR_SPEED_RAD_S = MAX_MOTOR_SPEED_RPM * M_PI / 30.0; // ~628.32 rad/s

// Tolerance for floating point comparisons
constexpr double EPSILON = 1e-3;

class TestActiveDifferential : public testing::Test
{
  protected:
    void SetUp() override
    {
        // Initialize CAN tables (required for app_vehicleDynamics.h dependencies)
        app_canTx_init();
        app_canRx_init();
    }
};

/**
 * Test straight driving (steering angle = 0)
 * All wheels should have the same speed
 */
TEST_F(TestActiveDifferential, StraightDriving)
{
    const double omega_v_ref        = 10.0; // rad/s
    const double steering_angle_deg = 0.0;
    const double omega_m_max        = 0.0; // Use default limit

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // All wheels should have the same speed
    const double expected_motor_speed = omega_v_ref * static_cast<double>(GEAR_RATIO);
    EXPECT_NEAR(result.front_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.front_right_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_right_torque, expected_motor_speed, EPSILON);
}

/**
 * Test right turn (positive steering angle)
 * Left wheels (FL, RL) should be faster (outer wheels)
 * Right wheels (FR, RR) should be slower (inner wheels)
 */
TEST_F(TestActiveDifferential, RightTurn)
{
    const double omega_v_ref        = 10.0; // rad/s
    const double steering_angle_deg = 30.0; // Right turn
    const double omega_m_max        = 0.0;  // Use default limit

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Convert steering wheel angle to wheel angle
    const double delta = static_cast<double>(
        DEG_TO_RAD(static_cast<float>(steering_angle_deg) * static_cast<float>(APPROX_STEERING_TO_WHEEL_ANGLE)));

    // Compute expected wheel speed difference
    const double delta_omega =
        (static_cast<double>(TRACK_WIDTH_m) / static_cast<double>(WHEELBASE_m)) * tan(delta) * omega_v_ref;

    // Left and right wheel references
    const double omega_L_wheel_ref = omega_v_ref + 0.5 * delta_omega;
    const double omega_R_wheel_ref = omega_v_ref - 0.5 * delta_omega;

    // Motor references
    const double omega_L_motor_ref = static_cast<double>(GEAR_RATIO) * omega_L_wheel_ref;
    const double omega_R_motor_ref = static_cast<double>(GEAR_RATIO) * omega_R_wheel_ref;

    // Right turn: left wheels are outer (faster), right wheels are inner (slower)
    EXPECT_NEAR(result.front_left_torque, omega_L_motor_ref, EPSILON);
    EXPECT_NEAR(result.rear_left_torque, omega_L_motor_ref, EPSILON);
    EXPECT_NEAR(result.front_right_torque, omega_R_motor_ref, EPSILON);
    EXPECT_NEAR(result.rear_right_torque, omega_R_motor_ref, EPSILON);

    // Verify left wheels are faster than right wheels
    EXPECT_GT(result.front_left_torque, result.front_right_torque);
    EXPECT_GT(result.rear_left_torque, result.rear_right_torque);
}

/**
 * Test left turn (negative steering angle)
 * Right wheels (FR, RR) should be faster (outer wheels)
 * Left wheels (FL, RL) should be slower (inner wheels)
 */
TEST_F(TestActiveDifferential, LeftTurn)
{
    const double omega_v_ref        = 10.0;  // rad/s
    const double steering_angle_deg = -30.0; // Left turn
    const double omega_m_max        = 0.0;   // Use default limit

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Convert steering wheel angle to wheel angle
    const double delta = static_cast<double>(
        DEG_TO_RAD(static_cast<float>(steering_angle_deg) * static_cast<float>(APPROX_STEERING_TO_WHEEL_ANGLE)));

    // Compute expected wheel speed difference
    const double delta_omega =
        (static_cast<double>(TRACK_WIDTH_m) / static_cast<double>(WHEELBASE_m)) * tan(delta) * omega_v_ref;

    // Left and right wheel references
    const double omega_L_wheel_ref = omega_v_ref + 0.5 * delta_omega;
    const double omega_R_wheel_ref = omega_v_ref - 0.5 * delta_omega;

    // Motor references
    const double omega_L_motor_ref = static_cast<double>(GEAR_RATIO) * omega_L_wheel_ref;
    const double omega_R_motor_ref = static_cast<double>(GEAR_RATIO) * omega_R_wheel_ref;

    // Left turn: right wheels are outer (faster), left wheels are inner (slower)
    EXPECT_NEAR(result.front_left_torque, omega_L_motor_ref, EPSILON);
    EXPECT_NEAR(result.rear_left_torque, omega_L_motor_ref, EPSILON);
    EXPECT_NEAR(result.front_right_torque, omega_R_motor_ref, EPSILON);
    EXPECT_NEAR(result.rear_right_torque, omega_R_motor_ref, EPSILON);

    // Verify right wheels are faster than left wheels
    EXPECT_GT(result.front_right_torque, result.front_left_torque);
    EXPECT_GT(result.rear_right_torque, result.rear_left_torque);
}

/**
 * Test speed limiting with custom omega_m_max
 * Motor speeds should be clamped to the provided limit
 */
TEST_F(TestActiveDifferential, SpeedLimitingCustomLimit)
{
    const double omega_v_ref        = 100.0; // rad/s (very high)
    const double steering_angle_deg = 0.0;
    const double omega_m_max        = 100.0; // Custom limit in rad/s

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Expected motor speed without limiting would be 100 * 14.321 = 1432.1 rad/s
    // But should be clamped to 100 rad/s
    const double expected_motor_speed = omega_m_max;

    EXPECT_NEAR(result.front_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.front_right_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_right_torque, expected_motor_speed, EPSILON);

    // Verify all speeds are at or below the limit
    EXPECT_LE(result.front_left_torque, omega_m_max + EPSILON);
    EXPECT_LE(result.front_right_torque, omega_m_max + EPSILON);
    EXPECT_LE(result.rear_left_torque, omega_m_max + EPSILON);
    EXPECT_LE(result.rear_right_torque, omega_m_max + EPSILON);
}

/**
 * Test speed limiting with default MAX_MOTOR_SPEED_RAD_S
 * When omega_m_max <= 0, should use default limit
 */
TEST_F(TestActiveDifferential, SpeedLimitingDefaultLimit)
{
    const double omega_v_ref        = 100.0; // rad/s (very high)
    const double steering_angle_deg = 0.0;
    const double omega_m_max        = 0.0; // Use default limit

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Expected motor speed without limiting would be 100 * 14.321 = 1432.1 rad/s
    // But should be clamped to MAX_MOTOR_SPEED_RAD_S
    const double expected_motor_speed = MAX_MOTOR_SPEED_RAD_S;

    EXPECT_NEAR(result.front_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.front_right_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_right_torque, expected_motor_speed, EPSILON);

    // Verify all speeds are at or below the default limit
    EXPECT_LE(result.front_left_torque, MAX_MOTOR_SPEED_RAD_S + EPSILON);
    EXPECT_LE(result.front_right_torque, MAX_MOTOR_SPEED_RAD_S + EPSILON);
    EXPECT_LE(result.rear_left_torque, MAX_MOTOR_SPEED_RAD_S + EPSILON);
    EXPECT_LE(result.rear_right_torque, MAX_MOTOR_SPEED_RAD_S + EPSILON);
}

/**
 * Test negative speed limiting (reverse)
 * Motor speeds should be clamped to negative limit
 */
TEST_F(TestActiveDifferential, NegativeSpeedLimiting)
{
    const double omega_v_ref        = -100.0; // rad/s (reverse, very high)
    const double steering_angle_deg = 0.0;
    const double omega_m_max        = 100.0; // Custom limit in rad/s

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Expected motor speed without limiting would be -100 * 14.321 = -1432.1 rad/s
    // But should be clamped to -100 rad/s
    const double expected_motor_speed = -omega_m_max;

    EXPECT_NEAR(result.front_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.front_right_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_right_torque, expected_motor_speed, EPSILON);

    // Verify all speeds are at or above the negative limit
    EXPECT_GE(result.front_left_torque, -omega_m_max - EPSILON);
    EXPECT_GE(result.front_right_torque, -omega_m_max - EPSILON);
    EXPECT_GE(result.rear_left_torque, -omega_m_max - EPSILON);
    EXPECT_GE(result.rear_right_torque, -omega_m_max - EPSILON);
}

/**
 * Test with zero vehicle speed
 * All motor speeds should be zero
 */
TEST_F(TestActiveDifferential, ZeroSpeed)
{
    const double omega_v_ref        = 0.0;
    const double steering_angle_deg = 45.0; // Even with steering, speed should be zero
    const double omega_m_max        = 0.0;

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    EXPECT_NEAR(result.front_left_torque, 0.0, EPSILON);
    EXPECT_NEAR(result.front_right_torque, 0.0, EPSILON);
    EXPECT_NEAR(result.rear_left_torque, 0.0, EPSILON);
    EXPECT_NEAR(result.rear_right_torque, 0.0, EPSILON);
}

/**
 * Test with small steering angle
 * Should still produce differential speeds
 */
TEST_F(TestActiveDifferential, SmallSteeringAngle)
{
    const double omega_v_ref        = 10.0;
    const double steering_angle_deg = 5.0; // Small right turn
    const double omega_m_max        = 0.0;

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Left wheels should be faster than right wheels (right turn)
    EXPECT_GT(result.front_left_torque, result.front_right_torque);
    EXPECT_GT(result.rear_left_torque, result.rear_right_torque);

    // Front and rear wheels on same side should have same speed
    EXPECT_NEAR(result.front_left_torque, result.rear_left_torque, EPSILON);
    EXPECT_NEAR(result.front_right_torque, result.rear_right_torque, EPSILON);
}

/**
 * Test with large steering angle
 * Should produce significant differential speeds
 */
TEST_F(TestActiveDifferential, LargeSteeringAngle)
{
    const double omega_v_ref        = 10.0;
    const double steering_angle_deg = 90.0; // Large right turn
    const double omega_m_max        = 0.0;

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Left wheels should be significantly faster than right wheels
    EXPECT_GT(result.front_left_torque, result.front_right_torque);
    EXPECT_GT(result.rear_left_torque, result.rear_right_torque);

    // Front and rear wheels on same side should have same speed
    EXPECT_NEAR(result.front_left_torque, result.rear_left_torque, EPSILON);
    EXPECT_NEAR(result.front_right_torque, result.rear_right_torque, EPSILON);
}

/**
 * Test speed limiting during turn
 * Outer wheels may hit limit while inner wheels don't
 */
TEST_F(TestActiveDifferential, SpeedLimitingDuringTurn)
{
    const double omega_v_ref        = 50.0;  // rad/s
    const double steering_angle_deg = 30.0;  // Right turn
    const double omega_m_max        = 200.0; // Limit that may be hit by outer wheels

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Convert steering wheel angle to wheel angle
    const double delta = static_cast<double>(
        DEG_TO_RAD(static_cast<float>(steering_angle_deg) * static_cast<float>(APPROX_STEERING_TO_WHEEL_ANGLE)));
    const double delta_omega =
        (static_cast<double>(TRACK_WIDTH_m) / static_cast<double>(WHEELBASE_m)) * tan(delta) * omega_v_ref;
    const double omega_L_wheel_ref           = omega_v_ref + 0.5 * delta_omega;
    const double omega_R_wheel_ref           = omega_v_ref - 0.5 * delta_omega;
    const double omega_L_motor_ref_unclamped = static_cast<double>(GEAR_RATIO) * omega_L_wheel_ref;
    const double omega_R_motor_ref_unclamped = static_cast<double>(GEAR_RATIO) * omega_R_wheel_ref;

    // Left wheels (outer) should be clamped if they exceed limit
    if (omega_L_motor_ref_unclamped > omega_m_max)
    {
        EXPECT_NEAR(result.front_left_torque, omega_m_max, EPSILON);
        EXPECT_NEAR(result.rear_left_torque, omega_m_max, EPSILON);
    }
    else
    {
        EXPECT_NEAR(result.front_left_torque, omega_L_motor_ref_unclamped, EPSILON);
        EXPECT_NEAR(result.rear_left_torque, omega_L_motor_ref_unclamped, EPSILON);
    }

    // Right wheels (inner) should be clamped if they exceed limit
    if (omega_R_motor_ref_unclamped > omega_m_max)
    {
        EXPECT_NEAR(result.front_right_torque, omega_m_max, EPSILON);
        EXPECT_NEAR(result.rear_right_torque, omega_m_max, EPSILON);
    }
    else
    {
        EXPECT_NEAR(result.front_right_torque, omega_R_motor_ref_unclamped, EPSILON);
        EXPECT_NEAR(result.rear_right_torque, omega_R_motor_ref_unclamped, EPSILON);
    }

    // All speeds should be within limits
    EXPECT_LE(result.front_left_torque, omega_m_max + EPSILON);
    EXPECT_GE(result.front_left_torque, -omega_m_max - EPSILON);
    EXPECT_LE(result.front_right_torque, omega_m_max + EPSILON);
    EXPECT_GE(result.front_right_torque, -omega_m_max - EPSILON);
}

/**
 * Test algorithm correctness: verify the mathematical relationship
 * Δω = (TRACK_WIDTH_m/WHEELBASE_m) * tan(δ) * ω_v
 */
TEST_F(TestActiveDifferential, AlgorithmCorrectness)
{
    const double omega_v_ref        = 15.0;
    const double steering_angle_deg = 20.0;
    const double omega_m_max        = 0.0;

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Convert steering wheel angle to wheel angle
    const double delta = static_cast<double>(
        DEG_TO_RAD(static_cast<float>(steering_angle_deg) * static_cast<float>(APPROX_STEERING_TO_WHEEL_ANGLE)));

    // Compute expected wheel speed difference (Eq. 13)
    const double delta_omega =
        (static_cast<double>(TRACK_WIDTH_m) / static_cast<double>(WHEELBASE_m)) * tan(delta) * omega_v_ref;

    // Left and right wheel references (Eq. 15-16)
    const double omega_L_wheel_ref = omega_v_ref + 0.5 * delta_omega;
    const double omega_R_wheel_ref = omega_v_ref - 0.5 * delta_omega;

    // Motor references (Eq. 17-18)
    const double omega_L_motor_ref = static_cast<double>(GEAR_RATIO) * omega_L_wheel_ref;
    const double omega_R_motor_ref = static_cast<double>(GEAR_RATIO) * omega_R_wheel_ref;

    // Verify results match expected calculations
    EXPECT_NEAR(result.front_left_torque, omega_L_motor_ref, EPSILON);
    EXPECT_NEAR(result.rear_left_torque, omega_L_motor_ref, EPSILON);
    EXPECT_NEAR(result.front_right_torque, omega_R_motor_ref, EPSILON);
    EXPECT_NEAR(result.rear_right_torque, omega_R_motor_ref, EPSILON);
}

/**
 * Test that front and rear wheels on the same side have identical speeds
 * This is a key characteristic of the 4WD differential algorithm
 */
TEST_F(TestActiveDifferential, FrontRearWheelConsistency)
{
    const double omega_v_ref        = 10.0;
    const double steering_angle_deg = 25.0;
    const double omega_m_max        = 0.0;

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Front and rear wheels on left side should be identical
    EXPECT_NEAR(result.front_left_torque, result.rear_left_torque, EPSILON);

    // Front and rear wheels on right side should be identical
    EXPECT_NEAR(result.front_right_torque, result.rear_right_torque, EPSILON);
}

/**
 * Test with negative omega_m_max (should use default limit)
 */
TEST_F(TestActiveDifferential, NegativeOmegaMaxUsesDefault)
{
    const double omega_v_ref        = 100.0;
    const double steering_angle_deg = 0.0;
    const double omega_m_max        = -10.0; // Negative, should use default

    TorqueAllocationOutputs result = app_activeDifferential_computeTorque(omega_v_ref, steering_angle_deg, omega_m_max);

    // Should use default MAX_MOTOR_SPEED_RAD_S limit
    const double expected_motor_speed = MAX_MOTOR_SPEED_RAD_S;

    EXPECT_NEAR(result.front_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.front_right_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_left_torque, expected_motor_speed, EPSILON);
    EXPECT_NEAR(result.rear_right_torque, expected_motor_speed, EPSILON);
}
