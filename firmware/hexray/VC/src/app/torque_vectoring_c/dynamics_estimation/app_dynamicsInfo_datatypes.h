#pragma once

#include <stdlib.h>
#include "app_imu.h"
#include "io_imu_config.h"
#include "app_pid.h"
#include "app_units.h"
#include "torque_vectoring_c/datatypes/app_vd_datatypes.h"
#include "app_sbgEllipse.h"

typedef struct
{
    double fric_coeff_fl;
    double fric_coeff_fr;
    double fric_coeff_rl;
    double fric_coeff_rr;
} road_tire_fric_coeff;
typedef struct
{
    double normalized_vx_fl;
    double normalized_vx_fr;
    double normalized_vx_rl;
    double normalized_vx_rr;
} longitudinal_vehicle_speed;

typedef struct
{
    double wheel_speed_fl;
    double wheel_speed_fr;
    double wheel_speed_rl;
    double wheel_speed_rr;
} speeds;

typedef struct
{
    double SA_fl;
    double SA_fr;
    double SA_rl;
    double SA_rr;
} slip_angles;

typedef struct
{
    double eff_rad_fl;
    double eff_rad_fr;
    double eff_rad_rl;
    double eff_rad_rr;
} eff_rads;

typedef struct
{
    double front_body_slip;
    double rear_body_slip;
} body_slip_angles;
typedef struct
{
    slip_ratios          slip_ratios;
    slip_angles          slip_angles;
    eff_rads             eff_rads;
    tire_forces          normal_forces;
    road_tire_fric_coeff tire_fric_coeffs;
} tire_info;

typedef struct
{
    speeds                      wheel_speeds;
    longitudinal_vehicle_speed  wheel_center_vx;
    const ImuData              *imu_data;
    vehicle_velocity_components vel_components;
} dynamicsEst_inputs;

typedef struct
{
    double           lat_loadTransfer;
    double           long_loadTransfer;
    tire_info        tireModel;
    body_slip_angles axle_body_slip;
    double           Fx_drag;       // TODO: Future consideration for drag force
    double           Fz_down_force; // TODO: Future consideration for down force
} dynamicsEst_outputs;              // not sure what to name this yet ... originally had it as tire
                                    // model when normal forces were in it but decided to take that out
                                    // as too many structs in structs
