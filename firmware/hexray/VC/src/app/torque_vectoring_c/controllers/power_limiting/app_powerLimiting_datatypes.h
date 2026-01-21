#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "torque_vectoring_c/datatypes/app_vd_datatypes.h"

typedef struct
{
    VD_TorqueToInv torques;
    VD_WheelRpms   wheel_rpms;
    double         regen_degrating_value;
    double         bms_power_limit;
    bool           isRegen_on;
} powerLimiting_inputs;

typedef struct
{
    double per_wheel_torque_reduction;
    double power_limit;
    double total_requested_power;
} powerLimiting_outputs;