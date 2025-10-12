#include "app_activeDifferential.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_units.h"

#include <math.h>

static float wheelAngleToSpeedDelta(const float wheel_angle_deg)
{
    // equation below derived from https://ieeexplore.ieee.org/document/6635706 equation 11
    // angle > 0 = right, angle < 0 = left
    return TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle_deg)) / (2 * WHEELBASE_mm);
}

TorqueAllocationOutputs app_activeDifferential_computeTorque(
    const double wheel_angle_deg,
    const bool   is_regen_mode,
    const double requested_torque_Nm)
{
    const float delta = wheelAngleToSpeedDelta(wheel_angle_deg);
    const float cl = is_regen_mode ? 1 - delta : 1 + delta, cr = is_regen_mode ? 1 + delta : 1 - delta;
    return (TorqueAllocationOutputs){
        requested_torque_Nm * cl,
        requested_torque_Nm * cr,
        requested_torque_Nm * cl,
        requested_torque_Nm * cr,
    };
}
