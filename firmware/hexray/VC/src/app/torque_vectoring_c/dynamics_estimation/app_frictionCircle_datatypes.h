#pragma once
#include <stdlib.h>
#include "app_dynamicsInfo_datatypes.h"
#include "torque_vectoring_c/datatypes/app_vd_datatypes.h"
#include "io/io_imu_config.h"
#include "app_vehicleDynamicsConstants.h"

typedef struct VD_frictionCircle_inputs
{
    ImuData              imu_data;
    double               desired_yawRate;
    double               desired_yawMoment;
    double               vehicle_velocity;
    double               body_slip;
    road_tire_fric_coeff tire_fric_coeffs;
    tire_forces          tire_normal_forces;
} VD_frictionCircle_inputs;
typedef struct
{
    slip_ratios allowable_slip_ratio;
    tire_forces required_Fy;
    tire_forces avail_Fx;
    tire_forces maximum_tractive_forces;

} VD_frictionCircle_outputs;
