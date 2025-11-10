#pragma once

#include <stdlib.h>
#include "app_imu.h"
#include "io_imu_config.h"
#include "app_pid.h"
#include "app_units.h"

typedef struct
{
    // Slip Ratio
    double SR_fl;
    double SR_fr;
    double SR_rl; 
    double SR_rr;
    // Slip Angle
    double SA_fl;
    double SA_fr;
    double SA_rl; 
    double SA_rr; 
    // effective radius
    double eff_rad_fl;
    double eff_rad_fr;
    double eff_rad_rl;
    double eff_rad_rr; 
    // Normal forces
    double Fz_fl;
    double Fz_fr;
    double Fz_rl;
    double Fz_rr;
}  VD_tire_info; 

typedef struct
{
    double wheel_speed_fl;
    double wheel_speed_fr; 
    double wheel_speed_rl;
    double wheel_speed_rr; 
    double vehicle_speed;
    const ImuData *imu_data;
} VD_dynamicsEst_inputs;


typedef struct
{
    double lat_loadTransfer; 
    double long_loadTransfer; 
    VD_tire_info tireModel;
    double Fx_drag; // TODO: Future consideration for drag force
    double Fz_down_force; // TODO: Future consideration for down force
} VD_dynamicsEst_outputs; // not sure what to name this yet ... originally had it as tire
                       // model when normal forces were in it but decided to take that out 
                       // as too many structs in structs

