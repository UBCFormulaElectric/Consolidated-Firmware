#pragma once
#include "app_vehicleDynamics.h"

void app_torqueVectoring_init(void);
void app_torqueVectoring_run(const float accelerator_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors);
// void app_torqueVectoring_handleAcceleration(void);
