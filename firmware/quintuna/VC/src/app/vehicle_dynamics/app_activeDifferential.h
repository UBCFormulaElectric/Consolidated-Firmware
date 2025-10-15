#pragma once
#include "app_vehicleDynamics.h"
#include <stdbool.h>

TorqueAllocationOutputs
    app_activeDifferential_computeTorque(double wheel_angle_deg, bool is_regen_mode, double requested_torque_Nm);
