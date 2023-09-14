#pragma once
#include "torquevectoring/App_TorqueVectoring.h"

void App_TractionControl_ComputeTorque(TractionControl_Inputs *inputs, TractionControl_Outputs *outputs);

float App_TractionControl_ComputeSlip(float motor_speed_rpm, float front_wheel_speed_rpm);

float App_TractionControl_WheelSpeedKPHToRPM(float speed_kph);
