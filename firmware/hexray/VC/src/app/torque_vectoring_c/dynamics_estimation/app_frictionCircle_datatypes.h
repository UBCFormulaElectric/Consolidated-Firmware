#pragma once
#include <stdlib.h>
#include "app_frictionCircle_datatypes.h"

typedef struct
{
    // for a desired yaw what is the lateral force required per wheel
    double required_Fy_fr;
    double required_Fy_fl;
    double required_Fy_rr;
    double required_Fy_rl;
    // based on required lateral force, the available 
    double available_Fx_fl; 
    double available_Fx_fr;
    double available_Fx_rl;
    double available_Fx_rr;
} VD_frictionCircleEst;

typedef struct
{
    double desired_yawRate;
    double desired_yawMoment;
}VD_frictionCircle_inputs; 

typedef struct
{} VD_frictionCircle_outputs;



