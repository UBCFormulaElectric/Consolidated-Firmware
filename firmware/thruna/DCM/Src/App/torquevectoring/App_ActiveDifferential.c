#include "torquevectoring/App_ActiveDifferential.h"
#include "App_SharedConstants.h"
#include "App_SharedDcmConstants.h"
#include <math.h>
#include "App_SharedMacros.h"
#include <stdlib.h>

void App_ActiveDifferential_ComputeTorque(ActiveDifferential_Inputs *inputs, ActiveDifferential_Outputs *outputs)
{
    float Delta = App_ActiveDifferential_WheelAngleToSpeedDelta(inputs->wheel_angle_deg);
    float cl    = 1 + Delta;
    float cr    = 1 - Delta;

    float torque_lim_Nm = App_ActiveDifferential_PowerToTorque(
        inputs->power_max_kW, inputs->motor_speed_left_rpm, inputs->motor_speed_right_rpm, cl, cr);

    float torque_left_Nm  = torque_lim_Nm * (1 + Delta);
    float torque_right_Nm = torque_lim_Nm * (1 - Delta);
    float torque_max_Nm   = fmaxf(torque_left_Nm, torque_right_Nm);

    float scale = 1.0f;
    if (torque_max_Nm > MAX_TORQUE_REQUEST_NM)
    {
        scale = MAX_TORQUE_REQUEST_NM / torque_max_Nm;
    }

    outputs->torque_left_Nm  = torque_left_Nm * scale;
    outputs->torque_right_Nm = torque_right_Nm * scale;
}

float App_ActiveDifferential_WheelAngleToSpeedDelta(float wheel_angle_deg)
{
    // angle > 0 = right
    // angle < = left

    return TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle_deg)) / (2 * WHEELBASE_mm);
}

float App_ActiveDifferential_PowerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr)
{
    return (POWER_TO_TORQUE_CONVERSION_FACTOR * power_kW) /
           (left_motor_speed_rpm * cl + right_motor_speed_rpm * cr + SMALL_EPSILON);
}
