#pragma once
#include "app_vehicleDynamics.h"

void app_tractionControl_computeTorque(TractionControl_Inputs *inputs, TractionControl_Outputs *outputs);

float app_tractionControl_computeSlip(float motor_speed_rpm, float front_wheel_speed_rpm);
