#include <app_vehicleDynamics.h>

float app_wheelVerticleForces_broadcast(void);

float app_loadTransferConstant(float long_accel);

void app_torqueAllocation(TorqueAllocationInputs inputs, float loadTransferConst);