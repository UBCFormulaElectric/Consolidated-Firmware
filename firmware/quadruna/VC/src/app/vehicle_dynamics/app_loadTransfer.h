#include <app_vehicleDynamics.h>
#include "io_imu_config.h"

float app_wheelVerticleForces_broadcast(imuData imu);

float app_loadTransferConstant(float long_accel);

void app_torqueAllocation(TorqueAllocationInputs *inputs, float loadTransferConst);