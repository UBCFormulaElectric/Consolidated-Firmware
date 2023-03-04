#pragma once
#include "torquevectoring/App_TorqueVectoring.h"

void App_ActiveDifferential_ComputeTorque(ActiveDifferential_Inputs *inputs, ActiveDifferential_Outputs *outputs);

float App_ActiveDifferential_SteeringAngleToSpeedDelta(float steering_angle);

float App_ActiveDifferential_PowerToTorque(float power_kW, int left_motor_speed_rpm, int right_motor_speed_rpm, float cl, float cr);
