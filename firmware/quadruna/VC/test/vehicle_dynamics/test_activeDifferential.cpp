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
#include "app_activeDifferential.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "math.h"
#include "app_units.h"
}

class ActiveDifferentialTest : public testing::Test
{
};

TEST_F(ActiveDifferentialTest, torques_do_not_exceed_motor_torque_limit_while_turning_at_max_torque_request)
{
    struct ActiveDifferential_Inputs  active_diff_inputs_t1;
    struct ActiveDifferential_Outputs actual_active_diff_outputs_t1;
    float                             wheel_angle_t1          = 30.0;
    float                             wheel_speed_left_rpm_t1 = 135, wheel_speed_right_rpm_t1 = 135;
    float                             pedal_percentage = 1.0;
    float                             scale            = 1.0;
    // our torque based driving requires a requested torque into so we pass in torque_t1 as are requested torque
    float delta_t1      = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle_t1)) / (2 * WHEELBASE_mm);
    float cl_t1         = 1 + delta_t1;
    float cr_t1         = 1 - delta_t1;
    float torque_t1     = pedal_percentage * MAX_TORQUE_REQUEST_NM;
    float torque_lim_t1 = (POWER_TO_TORQUE_CONVERSION_FACTOR * POWER_LIMIT_CAR_kW) /
                          (wheel_speed_left_rpm_t1 * cl_t1 + wheel_speed_right_rpm_t1 * cr_t1 + SMALL_EPSILON);

    active_diff_inputs_t1 = {
        wheel_angle_t1, wheel_speed_left_rpm_t1, wheel_speed_right_rpm_t1, POWER_LIMIT_CAR_kW, 0.0, torque_t1
    };

    app_activeDifferential_computeTorque(&active_diff_inputs_t1, &actual_active_diff_outputs_t1);
    float expected_torque_left_t1  = cl_t1 * torque_t1;
    float expected_torque_right_t1 = cr_t1 * torque_t1;
    float max_torque_t1            = fmaxf(expected_torque_left_t1, expected_torque_right_t1);

    if (max_torque_t1 > torque_lim_t1)
    {
        scale = torque_lim_t1 / max_torque_t1;
    }

    expected_torque_left_t1 *= scale;
    expected_torque_right_t1 *= scale;

    float expected_ratio_t1   = cl_t1 / cr_t1;
    float calculated_ratio_t1 = expected_torque_left_t1 / expected_torque_right_t1;
    float actual_ratio_t1 =
        actual_active_diff_outputs_t1.torque_left_Nm / actual_active_diff_outputs_t1.torque_right_Nm;
    ASSERT_FLOAT_EQ(expected_torque_left_t1, actual_active_diff_outputs_t1.torque_left_Nm);
    ASSERT_FLOAT_EQ(expected_torque_right_t1, actual_active_diff_outputs_t1.torque_right_Nm);

    // We clamp in torque vectoring.c as a final clamp before sending torque to motors.
    // I believe it is best to keep it like that rather than clamping anything that leaves
    // active diff because we still might want to manipulate the number when it leaves active diff
    //  and clamping it would mess with the algo. These tests are commented out because they assume that
    //  the output of active diff is clamped. Previously this worked because our scale was with max torque
    // not limit based on power limit

    // ASSERT_TRUE(actual_active_diff_outputs_t1.torque_right_Nm <= MAX_TORQUE_REQUEST_NM);
    // ASSERT_TRUE(actual_active_diff_outputs_t1.torque_left_Nm <= MAX_TORQUE_REQUEST_NM);

    ASSERT_FLOAT_EQ(expected_ratio_t1, calculated_ratio_t1);
    ASSERT_FLOAT_EQ(expected_ratio_t1, actual_ratio_t1);
};

TEST_F(ActiveDifferentialTest, no_torques_while_turning_at_zero_power_limit)
{
    struct ActiveDifferential_Inputs  active_diff_inputs_t2;
    struct ActiveDifferential_Outputs actual_active_diff_outputs_t2;
    float                             wheel_angle_t2          = 30.0;
    float                             wheel_speed_left_rpm_t2 = 135, wheel_speed_right_rpm_t2 = 135;
    float                             pedal_percentage = 0.6;
    float                             torque_t2        = pedal_percentage * MAX_TORQUE_REQUEST_NM;

    active_diff_inputs_t2 = { wheel_angle_t2, wheel_speed_left_rpm_t2, wheel_speed_right_rpm_t2, 0.0, 0.0, torque_t2 };
    app_activeDifferential_computeTorque(&active_diff_inputs_t2, &actual_active_diff_outputs_t2);
    ASSERT_FLOAT_EQ(0.0, actual_active_diff_outputs_t2.torque_left_Nm);
    ASSERT_FLOAT_EQ(0.0, actual_active_diff_outputs_t2.torque_right_Nm);
};

TEST_F(ActiveDifferentialTest, torques_follow_expected_ratio_while_turning_right_at_nonzero_power_limit)
{
    struct ActiveDifferential_Inputs  active_diff_inputs_t3;
    struct ActiveDifferential_Outputs actual_active_diff_outputs_t3;
    float                             wheel_angle_t3          = 30.0;
    float                             wheel_speed_left_rpm_t3 = 135, wheel_speed_right_rpm_t3 = 135;
    float                             power_lim_t3     = 60.0;
    float                             pedal_percentage = 0.42;
    float                             scale            = 1.0f;
    // our torque based driving requires a requested torque into so we pass in torque_t3 as are requested torque
    float delta_t3      = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle_t3)) / (2 * WHEELBASE_mm);
    float cl_t3         = 1 + delta_t3;
    float cr_t3         = 1 - delta_t3;
    float torque_t3     = pedal_percentage * MAX_TORQUE_REQUEST_NM;
    float torque_lim_t3 = (POWER_TO_TORQUE_CONVERSION_FACTOR * power_lim_t3) /
                          (wheel_speed_left_rpm_t3 * cl_t3 + wheel_speed_right_rpm_t3 * cr_t3 + SMALL_EPSILON);

    active_diff_inputs_t3 = { wheel_angle_t3, wheel_speed_left_rpm_t3, wheel_speed_right_rpm_t3, power_lim_t3, 0.0,
                              torque_t3 };

    app_activeDifferential_computeTorque(&active_diff_inputs_t3, &actual_active_diff_outputs_t3);
    float expected_torque_left_t3  = cl_t3 * torque_t3;
    float expected_torque_right_t3 = cr_t3 * torque_t3;
    float max_torque_t3            = fmaxf(expected_torque_left_t3, expected_torque_right_t3);

    if (max_torque_t3 > torque_lim_t3)
    {
        scale = torque_lim_t3 / max_torque_t3;
    }

    expected_torque_left_t3 *= scale;
    expected_torque_right_t3 *= scale;

    ASSERT_FLOAT_EQ(expected_torque_left_t3, actual_active_diff_outputs_t3.torque_left_Nm);
    ASSERT_FLOAT_EQ(expected_torque_right_t3, actual_active_diff_outputs_t3.torque_right_Nm);

    // see first test comment

    // ASSERT_TRUE(actual_active_diff_outputs_t3.torque_right_Nm <= MAX_TORQUE_REQUEST_NM);
    // ASSERT_TRUE(actual_active_diff_outputs_t3.torque_left_Nm <= MAX_TORQUE_REQUEST_NM);

    ASSERT_TRUE(actual_active_diff_outputs_t3.torque_left_Nm > actual_active_diff_outputs_t3.torque_right_Nm);
};

TEST_F(ActiveDifferentialTest, torques_follow_expected_ratio_while_turning_left_at_nonzero_power_limit)
{
    struct ActiveDifferential_Inputs  active_diff_inputs_t4;
    struct ActiveDifferential_Outputs actual_active_diff_outputs_t4;
    float                             wheel_angle_t4          = -30.0;
    float                             wheel_speed_left_rpm_t4 = 135, wheel_speed_right_rpm_t4 = 135;
    float                             power_lim_t4     = 60.0;
    float                             pedal_percentage = 0.73;
    float                             scale            = 1.0;
    // our torque based driving requires a requested torque into so we pass in torque_t4 as are requested torque
    float delta_t4      = TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle_t4)) / (2 * WHEELBASE_mm);
    float cl_t4         = 1 + delta_t4;
    float cr_t4         = 1 - delta_t4;
    float torque_t4     = pedal_percentage * MAX_TORQUE_REQUEST_NM;
    float torque_lim_t4 = (POWER_TO_TORQUE_CONVERSION_FACTOR * power_lim_t4) /
                          (wheel_speed_left_rpm_t4 * cl_t4 + wheel_speed_right_rpm_t4 * cr_t4 + SMALL_EPSILON);

    active_diff_inputs_t4 = { wheel_angle_t4, wheel_speed_left_rpm_t4, wheel_speed_right_rpm_t4, power_lim_t4, 0.0,
                              torque_t4 };

    app_activeDifferential_computeTorque(&active_diff_inputs_t4, &actual_active_diff_outputs_t4);
    float expected_torque_left_t4  = cl_t4 * torque_t4;
    float expected_torque_right_t4 = cr_t4 * torque_t4;
    float max_torque_t4            = fmaxf(expected_torque_left_t4, expected_torque_right_t4);

    if (max_torque_t4 > torque_lim_t4)
    {
        scale = torque_lim_t4 / max_torque_t4;
    }

    expected_torque_left_t4 *= scale;
    expected_torque_right_t4 *= scale;

    ASSERT_FLOAT_EQ(expected_torque_left_t4, actual_active_diff_outputs_t4.torque_left_Nm);
    ASSERT_FLOAT_EQ(expected_torque_right_t4, actual_active_diff_outputs_t4.torque_right_Nm);

    // see first test comment

    // ASSERT_TRUE(actual_active_diff_outputs_t4.torque_right_Nm <= MAX_TORQUE_REQUEST_NM);
    // ASSERT_TRUE(actual_active_diff_outputs_t4.torque_left_Nm <= MAX_TORQUE_REQUEST_NM);

    ASSERT_TRUE(actual_active_diff_outputs_t4.torque_left_Nm < actual_active_diff_outputs_t4.torque_right_Nm);
};