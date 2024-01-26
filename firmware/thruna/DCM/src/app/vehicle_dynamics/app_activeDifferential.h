#pragma once
#include "App_TorqueVectoring.h"
#include "app_vehicleDynamics.h"

void app_activeDifferential_computeTorque(ActiveDifferential_Inputs *inputs, ActiveDifferential_Outputs *outputs);

float app_activeDifferential_wheelAngleToSpeedDelta(float wheel_angle);

float app_activeDifferential_powerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr);
