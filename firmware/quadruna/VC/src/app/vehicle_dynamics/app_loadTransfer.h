#pragma once
#include <app_vehicleDynamics.h>
#include "io_imu_config.h"

void app_wheelVerticalForces_broadcast(const ImuData *imu);

float app_loadTransferConstant(float long_accel);

void app_torqueAllocation(TorqueAllocationInputs *inputs, float loadTransferConst);