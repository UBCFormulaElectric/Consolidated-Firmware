#pragma once
#include "torquevectoring/App_TorqueVectoring.h"

float ComputeSlip(float motor_speed_rpm, float front_wheel_speed_rpm);

void App_TractionControl_ComputeTorque(TractionControl_Inputs *inputs, TractionControl_Outputs *outputs);
