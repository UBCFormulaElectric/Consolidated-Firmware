#pragma once
#include <stdbool.h>
#include "app_vd_datatypes.h"


powerLimiting_outputs app_powerLimiting(VD_TorqueToInv torques, VD_WheelRpms wheel_rpms);
