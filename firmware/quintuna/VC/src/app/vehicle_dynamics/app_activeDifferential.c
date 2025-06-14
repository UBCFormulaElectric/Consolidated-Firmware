#include "app_activeDifferential.h"
#include "app_vehicleDynamicsConstants.h"
#include <math.h>
#include "app_units.h"
#include "app_utils.h"
#include <stdlib.h>

void app_activeDifferential_computeTorque(ActiveDifferential_Inputs *inputs, ActiveDifferential_Outputs *outputs)
{
    float Delta = app_activeDifferential_wheelAngleToSpeedDelta(inputs->wheel_angle_deg);
    float cl, cr;

    // TODO: Someone help me double check this pls
    if (inputs->is_regen_mode)
    {
        cl = 1 - Delta;
        cr = 1 + Delta;
    }
    else
    {
        cl = 1 + Delta;
        cr = 1 - Delta;
    }

    float torque_lim_Nm = app_activeDifferential_powerToTorque(
        inputs->power_max_kW, inputs->motor_speed_fl_rpm, inputs->motor_speed_fr_rpm, inputs->motor_speed_rl_rpm,
        inputs->motor_speed_rr_rpm, cl, cr);

    float torque_left_Nm         = inputs->requested_torque_Nm * cl;
    float torque_right_Nm        = inputs->requested_torque_Nm * cr;
    float torque_max_Nm          = fmaxf(torque_left_Nm, torque_right_Nm);
    float torque_negative_max_Nm = fminf(torque_left_Nm, torque_right_Nm);

    float scale = CLAMP_TO_ONE(inputs->derating_value);

    if (inputs->is_regen_mode && (torque_negative_max_Nm < -torque_lim_Nm))
    {
        scale *= -torque_lim_Nm / torque_negative_max_Nm;
    }
    else if (!inputs->is_regen_mode && (torque_max_Nm > torque_lim_Nm))
    {
        scale = torque_lim_Nm / torque_max_Nm;
    }

    outputs->torque_fl_Nm = torque_left_Nm * scale;
    outputs->torque_fr_Nm = torque_right_Nm * scale;
    outputs->torque_rl_Nm = torque_left_Nm * scale;
    outputs->torque_rr_Nm = torque_right_Nm * scale;
}

float app_activeDifferential_wheelAngleToSpeedDelta(float wheel_angle_deg)
{
    // equation below derived from https://ieeexplore.ieee.org/document/6635706 equation 11
    // angle > 0 = right, angle < 0 = left

    return TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle_deg)) / (2 * WHEELBASE_mm);
}

float app_activeDifferential_powerToTorque(
    float power_kW,
    float motor_speed_fl_rpm,
    float motor_speed_fr_rpm,
    float motor_speed_rl_rpm,
    float motor_speed_rr_rpm,
    float cl,
    float cr)
{
    // TODO: Change conversion factor for AMK inverters
    return (POWER_TO_TORQUE_CONVERSION_FACTOR * power_kW) /
           ((motor_speed_fl_rpm + motor_speed_fr_rpm) * cl + (motor_speed_rl_rpm + motor_speed_rr_rpm) * cr +
            SMALL_EPSILON);
}
