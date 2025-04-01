#include "app_activeDifferential.h"
#include "app_vehicleDynamicsConstants.h"
#include <math.h>
#include "app_units.h"
#include "app_utils.h"
#include <stdlib.h>

void app_activeDifferential_computeTorque(ActiveDifferential_Inputs *inputs, ActiveDifferential_Outputs *outputs)
{
    float Delta = app_activeDifferential_wheelAngleToSpeedDelta(inputs->wheel_angle_deg);
    float cl    = 1 + Delta;
    float cr    = 1 - Delta;

    float torque_lim_Nm = app_activeDifferential_powerToTorque(
        inputs->power_max_kW, inputs->motor_speed_left_rpm, inputs->motor_speed_right_rpm, cl, cr);

    float torque_left_Nm  = inputs->requested_torque * (1 + Delta);
    float torque_right_Nm = inputs->requested_torque * (1 - Delta);
    float torque_max_Nm   = fmaxf(torque_left_Nm, torque_right_Nm);

    float scale = 1.0f;
    if (torque_max_Nm > torque_lim_Nm)
    {
        scale = torque_lim_Nm / torque_max_Nm;
    }

    outputs->torque_left_Nm  = torque_left_Nm * scale;
    outputs->torque_right_Nm = torque_right_Nm * scale;
}

float app_activeDifferential_wheelAngleToSpeedDelta(float wheel_angle_deg)
{
    // equation below derrived from https://ieeexplore.ieee.org/document/6635706 equation 11
    // angle > 0 = right5
    // angle < = left

    return TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle_deg)) / (2 * WHEELBASE_mm);
}

float app_activeDifferential_powerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr)
{
    return (POWER_TO_TORQUE_CONVERSION_FACTOR * power_kW) /
           (left_motor_speed_rpm * cl + right_motor_speed_rpm * cr + SMALL_EPSILON);
}
