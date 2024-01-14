#pragma once
#include "App_TorqueVectoring.h"
#include "App_VehicleDynamicsControl.h"

void App_ActiveDifferential_ComputeTorque(ActiveDifferential_Inputs *inputs, ActiveDifferential_Outputs *outputs);

float App_ActiveDifferential_WheelAngleToSpeedDelta(float wheel_angle);

float App_ActiveDifferential_PowerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr);
