#pragma once
#include "app_torqueVectoring.h"
#include "app_vehicleDynamics.h"

void app_activeDifferential_computeTorque(ActiveDifferential_Inputs *inputs, ActiveDifferential_Outputs *outputs);

float app_activeDifferential_wheelAngleToSpeedDelta(float wheel_angle);

float app_activeDifferential_powerToTorque(
    float power_kW,
    float motor_speed_rr_rpm,
    float motor_speed_rl_rpm,
    float motor_speed_fr_rpm,
    float motor_speed_fl_rpm,
    float cl,
    float cr);
