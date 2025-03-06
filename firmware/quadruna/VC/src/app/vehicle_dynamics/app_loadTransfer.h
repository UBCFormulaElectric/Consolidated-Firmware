#include <app_vehicleDynamics.h>

float app_wheelVerticleForces_broadcast(void);

float app_loadTransferConstant(float long_accel);

void torqueAllocation(TorqueAllocationInputs *inputs, float loadTransferConst);