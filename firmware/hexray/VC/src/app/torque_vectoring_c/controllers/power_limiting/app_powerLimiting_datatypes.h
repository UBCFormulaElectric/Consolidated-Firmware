#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "app_vd_datatypes.h"

typedef struct
{
    VD_WheelRpms wheel_rpms double regen_degrating_value;
    torque_to_inv                  torques;
    double                         bms_power_limit;
    bool                           isRegen_on;
} powerLimiting_inputs;

typedef struct
{
    double per_wheel_torque_reduction;
    double power_limit;
    double total_requested_power;
} powerLimiting_outputs;