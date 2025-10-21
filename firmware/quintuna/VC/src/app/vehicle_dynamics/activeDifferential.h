#pragma once
#include "app/vehicle_dynamics/torqueVectoring.h"
#include "app/vehicle_dynamics/vehicleDynamics.h"

void app_activeDifferential_computeTorque(ActiveDifferential_Inputs *inputs, TorqueAllocationOutputs *outputs);

float app_activeDifferential_wheelAngleToSpeedDelta(float wheel_angle_deg);
