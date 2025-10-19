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
#include "utils.h"
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

    float vehicle_speed  = 30.0f;
    float steering_angle = 30.0f;

    yrc.wheel_angle_rad      = DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE);
    yrc.vehicle_velocity_mps = KMH_TO_MPS(vehicle_speed);

    float ref_yaw_rate_rad_test =
        (yrc.vehicle_velocity_mps * yrc.wheel_angle_rad) /
        ((WHEELBASE_mm * MM_TO_M) + YAW_RATE_CONTROLLER_CONFIG.ku * SQUARE(yrc.vehicle_velocity_mps));

    app_yawRateController_computeRefYawRate(&yrc);

    ASSERT_FLOAT_EQ(ref_yaw_rate_rad_test, app_yawRateController_getRefYawRateRad());
};

TEST_F(YawRateControllerTest, no_yaw_rate_generation_at_zero_speed)
{
    YawRateController yrc;
    PID               pid;

    app_yawRateController_init(&yrc, &pid, &YAW_RATE_CONTROLLER_CONFIG);

    float vehicle_speed  = 0.0f;
    float steering_angle = 30.0f;

    yrc.wheel_angle_rad      = DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE);
    yrc.vehicle_velocity_mps = KMH_TO_MPS(vehicle_speed);

    float ref_yaw_rate_rad_test =
        (yrc.vehicle_velocity_mps * yrc.wheel_angle_rad) /
        ((WHEELBASE_mm * MM_TO_M) + YAW_RATE_CONTROLLER_CONFIG.ku * SQUARE(yrc.vehicle_velocity_mps));

    app_yawRateController_computeRefYawRate(&yrc);

    ASSERT_FLOAT_EQ(ref_yaw_rate_rad_test, app_yawRateController_getRefYawRateRad());
};

TEST_F(YawRateControllerTest, no_yaw_rate_generation_at_zero_steering_angle)
{
    YawRateController yrc;
    PID               pid;

    app_yawRateController_init(&yrc, &pid, &YAW_RATE_CONTROLLER_CONFIG);

    float vehicle_speed  = 30.0f;
    float steering_angle = 0.0f;

    yrc.wheel_angle_rad      = DEG_TO_RAD(steering_angle * APPROX_STEERING_TO_WHEEL_ANGLE);
    yrc.vehicle_velocity_mps = KMH_TO_MPS(vehicle_speed);

    float ref_yaw_rate_rad_test =
        (yrc.vehicle_velocity_mps * yrc.wheel_angle_rad) /
        ((WHEELBASE_mm * MM_TO_M) + YAW_RATE_CONTROLLER_CONFIG.ku * SQUARE(yrc.vehicle_velocity_mps));

    app_yawRateController_computeRefYawRate(&yrc);

    ASSERT_FLOAT_EQ(ref_yaw_rate_rad_test, app_yawRateController_getRefYawRateRad());
};