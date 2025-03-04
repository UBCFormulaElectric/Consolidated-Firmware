/**
 * Notes:
 * extern "C": specifies C linkage for functions to compiler
 *
 *
 *
 */

#include <gtest/gtest.h>

extern "C"
{
#include "app_utils.h"
#include "app_powerLimiting.h"
#include "app_yawRateController.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "app_pid.h"
#include "math.h"
#include "app_units.h"
}

class YawRateControllerTest : public testing::Test
{
};

TEST_F(YawRateControllerTest, basic_yaw_rate_generation)
{
    YawRateController yrc;
    PID               pid;

    app_yawRateController_init(&yrc, &pid, &YAW_RATE_CONTROLLER_CONFIG);

    float vehicle_speed = 30.0f;
    float steering_angle = 30.0f;

    yrc.wheel_angle_deg  = steering_angle;
    yrc.vehicle_velocity_kmh = vehicle_speed;

    float ref_yaw_rate_rad_test = (KMH_TO_MPS(vehicle_speed) * DEG_TO_RAD(steering_angle * 0.3)) / ((WHEELBASE_mm * MM_TO_M) + YAW_RATE_CONTROLLER_CONFIG.ku * (KMH_TO_MPS(vehicle_speed) * KMH_TO_MPS(vehicle_speed)));
    float ref_yaw_rate_deg_test = RAD_TO_DEG(ref_yaw_rate_rad_test);

    app_yawRateController_computeRefYawRate(&yrc);

    ASSERT_FLOAT_EQ(ref_yaw_rate_deg_test, yrc.ref_yaw_rate_deg);
};

TEST_F(YawRateControllerTest, no_yaw_rate_generation_at_zero_speed)
{
    YawRateController yrc;
    PID               pid;

    app_yawRateController_init(&yrc, &pid, &YAW_RATE_CONTROLLER_CONFIG);

    float vehicle_speed = 0.0f;
    float steering_angle = 30.0f;

    yrc.wheel_angle_deg  = steering_angle;
    yrc.vehicle_velocity_kmh = vehicle_speed;

    float ref_yaw_rate_rad_test = (KMH_TO_MPS(vehicle_speed) * DEG_TO_RAD(steering_angle * 0.3)) / ((WHEELBASE_mm * MM_TO_M) + YAW_RATE_CONTROLLER_CONFIG.ku * (KMH_TO_MPS(vehicle_speed) * KMH_TO_MPS(vehicle_speed)));
    float ref_yaw_rate_deg_test = RAD_TO_DEG(ref_yaw_rate_rad_test);

    app_yawRateController_computeRefYawRate(&yrc);

    ASSERT_FLOAT_EQ(ref_yaw_rate_deg_test, yrc.ref_yaw_rate_deg);
};

TEST_F(YawRateControllerTest, no_yaw_rate_generation_at_zero_steering_angle)
{
    YawRateController yrc;
    PID               pid;

    app_yawRateController_init(&yrc, &pid, &YAW_RATE_CONTROLLER_CONFIG);

    float vehicle_speed = 30.0f;
    float steering_angle = 0.0f;

    yrc.wheel_angle_deg  = steering_angle;
    yrc.vehicle_velocity_kmh = vehicle_speed;

    float ref_yaw_rate_rad_test = (KMH_TO_MPS(vehicle_speed) * DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / ((WHEELBASE_mm * MM_TO_M) + YAW_RATE_CONTROLLER_CONFIG.ku * (KMH_TO_MPS(vehicle_speed) * KMH_TO_MPS(vehicle_speed)));
    float ref_yaw_rate_deg_test = RAD_TO_DEG(ref_yaw_rate_rad_test);

    app_yawRateController_computeRefYawRate(&yrc);

    ASSERT_FLOAT_EQ(ref_yaw_rate_deg_test, yrc.ref_yaw_rate_deg);
};

TEST_F(YawRateControllerTest, torque_distribution_lambda_is_one)
{
    YawRateController yrc;
    PID               pid;

    app_yawRateController_init(&yrc, &pid, &YAW_RATE_CONTROLLER_CONFIG);

    float vehicle_speed = 30.0f;
    float steering_angle = 30.0f;

    yrc.wheel_angle_deg  = steering_angle;
    yrc.vehicle_velocity_kmh = vehicle_speed;
    // steady state approx for yaw rate
    yrc.real_yaw_rate_deg = RAD_TO_DEG(KMH_TO_MPS(vehicle_speed) / ((WHEELBASE_mm * MM_TO_M) / (tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)))));
    yrc.requested_torque = 50.0f;

    float ref_yaw_rate_rad_test = (KMH_TO_MPS(vehicle_speed) * DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / ((WHEELBASE_mm * MM_TO_M) + YAW_RATE_CONTROLLER_CONFIG.ku * (KMH_TO_MPS(vehicle_speed) * KMH_TO_MPS(vehicle_speed)));
    float ref_yaw_rate_deg_test = RAD_TO_DEG(ref_yaw_rate_rad_test);
    app_yawRateController_computeRefYawRate(&yrc);

    ASSERT_FLOAT_EQ(ref_yaw_rate_deg_test, yrc.ref_yaw_rate_deg);

    app_yawRateController_pidCompute(&yrc);

    float yaw_rate_diff = fabsf(DEG_TO_RAD(yrc.ref_yaw_rate_deg)) - fabsf(DEG_TO_RAD(yrc.real_yaw_rate_deg));
    float lambda        = 0.0f;

    // if (yaw_rate_diff <= yrc.k4) {
    //     lambda = 0.0f;
    // } 
    // else if (yaw_rate_diff > yrc.k4 && yaw_rate_diff < yrc.k5) {
    //     lambda = (yaw_rate_diff - yrc.k4) / (yrc.k5 - yrc.k4);
    // } 
    // else if (yaw_rate_diff >= yrc.k5) {
    //     lambda = 1.0f;
    // }

    if (yaw_rate_diff > yrc.k4 && yaw_rate_diff < yrc.k5) {
        lambda = (yaw_rate_diff - yrc.k4) / (yrc.k5 - yrc.k4);
    } 
    else if (yaw_rate_diff >= yrc.k5) {
        lambda = 1.0f;
    }

    float torque_left_Nm_test = (0.5f * yrc.requested_torque) + (0.5f * lambda * yrc.yaw_moment);
    float torque_right_Nm_test = (0.5f * yrc.requested_torque) - (0.5f * lambda * yrc.yaw_moment);
    std::cout << "ref_yaw_rate: " << yrc.ref_yaw_rate_deg << "real_yaw_rate" << yrc.real_yaw_rate_deg << "yaw_rate_diff" << yaw_rate_diff << "\n";
    std::cout << "ref_yaw_rate: " << yrc.ref_yaw_rate_deg << " yaw_moment: " << yrc.yaw_moment << " lambda: " << lambda << " torque_left: " << torque_left_Nm_test << " torque_right: " << torque_right_Nm_test << "\n";

    app_yawRateController_computeTorque(&yrc);

    std::cout << "torque_left: " << yrc.torque_left_Nm << " torque_right: " << yrc.torque_right_Nm << "\n";

    // ASSERT_FLOAT_EQ(torque_right_Nm_test, yrc.torque_right_Nm);
    ASSERT_FLOAT_EQ(torque_left_Nm_test, yrc.torque_left_Nm);
    ASSERT_FLOAT_EQ(torque_right_Nm_test, yrc.torque_right_Nm);
};

TEST_F(YawRateControllerTest, torque_distribution_lambda_is_zero)
{
    YawRateController yrc;
    PID               pid;

    app_yawRateController_init(&yrc, &pid, &YAW_RATE_CONTROLLER_CONFIG);

    float vehicle_speed = 30.0f;
    float steering_angle = 5.0f;

    yrc.wheel_angle_deg  = steering_angle;
    yrc.vehicle_velocity_kmh = vehicle_speed;
    // steady state approx for yaw rate
    yrc.real_yaw_rate_deg = RAD_TO_DEG(KMH_TO_MPS(vehicle_speed) / ((WHEELBASE_mm * MM_TO_M) / (tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)))));
    yrc.requested_torque = 50.0f;

    float ref_yaw_rate_rad_test = (KMH_TO_MPS(vehicle_speed) * DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / ((WHEELBASE_mm * MM_TO_M) + YAW_RATE_CONTROLLER_CONFIG.ku * (KMH_TO_MPS(vehicle_speed) * KMH_TO_MPS(vehicle_speed)));
    float ref_yaw_rate_deg_test = RAD_TO_DEG(ref_yaw_rate_rad_test);
    app_yawRateController_computeRefYawRate(&yrc);

    ASSERT_FLOAT_EQ(ref_yaw_rate_deg_test, yrc.ref_yaw_rate_deg);

    app_yawRateController_pidCompute(&yrc);

    float yaw_rate_diff = fabsf(DEG_TO_RAD(yrc.ref_yaw_rate_deg)) - fabsf(DEG_TO_RAD(yrc.real_yaw_rate_deg));
    float lambda        = 0.0f;

    if (yaw_rate_diff <= yrc.k4) {
        lambda = 0.0f;
    } 
    else if (yaw_rate_diff > yrc.k4 && yaw_rate_diff < yrc.k5) {
        lambda = (yaw_rate_diff - yrc.k4) / (yrc.k5 - yrc.k4);
    } 
    else if (yaw_rate_diff >= yrc.k5) {
        lambda = 1.0f;
    }

    float torque_left_Nm_test = (0.5f * yrc.requested_torque) + (0.5f * lambda * yrc.yaw_moment);
    float torque_right_Nm_test = (0.5f * yrc.requested_torque) - (0.5f * lambda * yrc.yaw_moment);
    std::cout << "ref_yaw_rate: " << yrc.ref_yaw_rate_deg << "real_yaw_rate" << yrc.real_yaw_rate_deg << "yaw_rate_diff" << yaw_rate_diff << "\n";
    std::cout << "ref_yaw_rate: " << yrc.ref_yaw_rate_deg << " yaw_moment: " << yrc.yaw_moment << " lambda: " << lambda << " torque_left: " << torque_left_Nm_test << " torque_right: " << torque_right_Nm_test << "\n";

    app_yawRateController_computeTorque(&yrc);

    std::cout << "torque_left: " << yrc.torque_left_Nm << " torque_right: " << yrc.torque_right_Nm << "\n";

    // ASSERT_FLOAT_EQ(torque_right_Nm_test, yrc.torque_right_Nm);
    ASSERT_FLOAT_EQ(torque_left_Nm_test, yrc.torque_left_Nm);
    ASSERT_FLOAT_EQ(torque_right_Nm_test, yrc.torque_right_Nm);
};

TEST_F(YawRateControllerTest, torque_distribution_lambda_is_calculated)
{
    YawRateController yrc;
    PID               pid;

    app_yawRateController_init(&yrc, &pid, &YAW_RATE_CONTROLLER_CONFIG);

    float vehicle_speed = 30.0f;
    float steering_angle = 25.0f;

    yrc.wheel_angle_deg  = steering_angle;
    yrc.vehicle_velocity_kmh = vehicle_speed;
    // steady state approx for yaw rate
    yrc.real_yaw_rate_deg = RAD_TO_DEG(KMH_TO_MPS(vehicle_speed) / ((WHEELBASE_mm * MM_TO_M) / (tanf(DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)))));
    yrc.requested_torque = 50.0f;

    float ref_yaw_rate_rad_test = (KMH_TO_MPS(vehicle_speed) * DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE)) / ((WHEELBASE_mm * MM_TO_M) + YAW_RATE_CONTROLLER_CONFIG.ku * (KMH_TO_MPS(vehicle_speed) * KMH_TO_MPS(vehicle_speed)));
    float ref_yaw_rate_deg_test = RAD_TO_DEG(ref_yaw_rate_rad_test);
    app_yawRateController_computeRefYawRate(&yrc);

    ASSERT_FLOAT_EQ(ref_yaw_rate_deg_test, yrc.ref_yaw_rate_deg);

    app_yawRateController_pidCompute(&yrc);

    float yaw_rate_diff = fabsf(DEG_TO_RAD(yrc.ref_yaw_rate_deg)) - fabsf(DEG_TO_RAD(yrc.real_yaw_rate_deg));
    float lambda        = 0.0f;

    if (yaw_rate_diff <= yrc.k4) {
        lambda = 0.0f;
    } 
    else if (yaw_rate_diff > yrc.k4 && yaw_rate_diff < yrc.k5) {
        lambda = (yaw_rate_diff - yrc.k4) / (yrc.k5 - yrc.k4);
    } 
    else if (yaw_rate_diff >= yrc.k5) {
        lambda = 1.0f;
    }

    float torque_left_Nm_test = (0.5f * yrc.requested_torque) + (0.5f * lambda * yrc.yaw_moment);
    float torque_right_Nm_test = (0.5f * yrc.requested_torque) - (0.5f * lambda * yrc.yaw_moment);
    std::cout << "ref_yaw_rate: " << yrc.ref_yaw_rate_deg << "real_yaw_rate" << yrc.real_yaw_rate_deg << "yaw_rate_diff" << yaw_rate_diff << "\n";
    std::cout << "ref_yaw_rate: " << yrc.ref_yaw_rate_deg << " yaw_moment: " << yrc.yaw_moment << " lambda: " << lambda << " torque_left: " << torque_left_Nm_test << " torque_right: " << torque_right_Nm_test << "\n";

    app_yawRateController_computeTorque(&yrc);

    std::cout << "torque_left: " << yrc.torque_left_Nm << " torque_right: " << yrc.torque_right_Nm << "\n";

    // ASSERT_FLOAT_EQ(torque_right_Nm_test, yrc.torque_right_Nm);
    ASSERT_FLOAT_EQ(torque_left_Nm_test, yrc.torque_left_Nm);
    ASSERT_FLOAT_EQ(torque_right_Nm_test, yrc.torque_right_Nm);
};