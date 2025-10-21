#include "activeDifferential.h"
#include "vehicleDynamicsConstants.h"
#include <math.h>
#include "app/units.h"
#include "app/utils.h"

// static float powerToTorque(
//     const float power_kW,
//     const float motor_speed_fl_rpm,
//     const float motor_speed_fr_rpm,
//     const float motor_speed_rl_rpm,
//     const float motor_speed_rr_rpm,
//     const float cl,
//     const float cr)
// {
//     return POWER_TO_TORQUE_CONVERSION_FACTOR * power_kW /
//            ((motor_speed_fl_rpm + motor_speed_fr_rpm) * cl + (motor_speed_rl_rpm + motor_speed_rr_rpm) * cr +
//             SMALL_EPSILON);
// }

void app_activeDifferential_computeTorque(ActiveDifferential_Inputs *inputs, TorqueAllocationOutputs *outputs)
{
    const float Delta = app_activeDifferential_wheelAngleToSpeedDelta(inputs->wheel_angle_deg);
    const float cl = inputs->is_regen_mode ? 1 - Delta : 1 + Delta, cr = inputs->is_regen_mode ? 1 + Delta : 1 - Delta;

    outputs->front_left_torque  = inputs->requested_torque_Nm * cl;
    outputs->front_right_torque = inputs->requested_torque_Nm * cr;
    outputs->rear_left_torque   = inputs->requested_torque_Nm * cl;
    outputs->rear_right_torque  = inputs->requested_torque_Nm * cr;
}

float app_activeDifferential_wheelAngleToSpeedDelta(const float wheel_angle_deg)
{
    // equation below derived from https://ieeexplore.ieee.org/document/6635706 equation 11
    // angle > 0 = right, angle < 0 = left

    return TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle_deg)) / (2 * WHEELBASE_mm);
}
