#pragma once
#include "app_torqueVectoring.h"
#include "app_vehicleDynamics.h"

void app_activeDifferential_computeTorque(ActiveDifferential_Inputs *inputs, ActiveDifferential_Outputs *outputs);

float app_activeDifferential_wheelAngleToSpeedDelta(float wheel_angle_deg);
