#pragma once
#include <stdbool.h>
#include "torque_vectoring_c/datatypes/app_vd_datatypes.h"
#include "torque_vectoring_c/controllers/power_limiting/app_powerLimiting_datatypes.h"
#include "app_vehicleDynamics.h"

powerLimiting_outputs app_powerLimiting(VD_TorqueToInv torques, VD_WheelRpms wheel_rpms);

// Helper functions for compatibility with quintuna API
float app_powerLimiting_computeMaxPower(bool isRegenOn);
float app_totalPower(TorqueAllocationOutputs *torques);
void  app_powerLimiting_torqueReduction(PowerLimitingInputs *inputs);
