#pragma once
#include "app_frictionCircle_estimation.h"
#include "app_dynamicsInfo_datatypes.h"
#include "app_frictionCircle_datatypes.h"
#include "app_yawRateController.h"
#include "math.h"

#define TASK_RATE ((double)(10.0 / 1000)) // 10ms task

/************************* Private Function Prototype *****************************/

static tire_forces app_dynamicsInfo_requiredLateralForce(VD_frictionCircle_inputs inputs);
static inline tire_forces
    app_frictionCircle_calcMaxTractiveForce(road_tire_fric_coeff friction_coeffs, tire_forces normal_forces);
static inline tire_forces app_frictionCircle_calcAvailable_Fx(tire_forces max_tractive_force, tire_forces required_Fy);

/************************* Global Function Definition *****************************/

VD_frictionCircle_outputs app_frictionCircle_estimate()
{
    const VD_frictionCircle_inputs inputs = app_yawRateController_getFrictionCircleInputs();

    return (VD_frictionCircle_outputs){ .maximum_tractive_forces = app_frictionCircle_calcMaxTractiveForce(
                                            inputs.tire_fric_coeffs, inputs.tire_normal_forces),
                                        .required_Fy = app_frictionCircle_calcRequired_Fy(inputs),
                                        .avail_Fx =
                                            app_frictionCircle_calcAvailable_Fx(max_tractive_forces, required_Fy) };
}

/************************* Static Function Definition *****************************/
static VD_frictionCircle_inputs app_frictionCircle_inputs(
    ImuData              imu_data,
    double               desired_yawRate,
    double               desired_yawMoment,
    double               vehicle_velocity,
    double               body_slip,
    road_tire_fric_coeff tire_fric_coeffs,
    tire_forces          tire_normal_forces)
{
    return (VD_frictionCircle_inputs){ .imu_data           = imu_data,
                                       .desired_yawRate    = desired_yawRate,
                                       .desired_yawMoment  = desired_yawMoment,
                                       .vehicle_velocity   = vehicle_velocity,
                                       .body_slip          = body_slip,
                                       .tire_fric_coeffs   = tire_fric_coeffs,
                                       .tire_normal_forces = tire_normal_forces };
}

static void app_frictionCircle_broadcast(VD_frictionCircle_outputs out)
{
    app_canTx_VC_FrictionCircle_MaxTractiveForce_fl_set(out.maximum_tractive_forces.fl);
    app_canTx_VC_FrictionCircle_MaxTractiveForce_fr_set(out.maximum_tractive_forces.fr);
    app_canTx_VC_FrictionCircle_MaxTractiveForce_rl_set(out.maximum_tractive_forces.rl);
    app_canTx_VC_FrictionCircle_MaxTractiveForce_rr_set(out.maximum_tractive_forces.rr);

    app_canTx_VC_FrictionCircle_Required_Fy_fl_set(out.required_Fy.fl);
    app_canTx_VC_FrictionCircle_Required_Fy_fr_set(out.required_Fy.fr);
    app_canTx_VC_FrictionCircle_Required_Fy_rl_set(out.required_Fy.rl);
    app_canTx_VC_FrictionCircle_Required_Fy_rr_set(out.required_Fy.rr);

    app_canTx_VC_FrictionCircle_Avail_Fx_fl_set(out.avail_Fx.fl);
    app_canTx_VC_FrictionCircle_Avail_Fx_fr_set(out.avail_Fx.fr);
    app_canTx_VC_FrictionCircle_Avail_Fx_rl_set(out.avail_Fx.rl);
    app_canTx_VC_FrictionCircle_Avail_Fx_rr_set(out.avail_Fx.rr);
}

static inline tire_forces
    app_frictionCircle_calcMaxTractiveForce(road_tire_fric_coeff friction_coeffs, tire_forces normal_forces)
{
    return (tire_forces){
        .fl = friction_coeff.fric_coeff_fl * normal_forces.fl,
        .fr = friction_coeff.fric_coeff_fr * normal_forces.fr,
        .rl = friction_coeff.fric_coeff_rl * normal_forces.rl,
        .rr = friction_coeff.fric_coeff_rr * normal_forces.rr,
    };
}
// TODO: modify to accomodate load transfer
static tire_forces app_frictionCircle_calcRequired_Fy(VD_frictionCircle_inputs inputs)
{
    static double prev_body_slip           = 0.0;
    const double  body_slip_rate_of_change = inputs.body_slip - prev_body_slip / TASK_RATE;
    const double  body_lat_force =
        (CAR_MASS_AT_CG_KG * inputs.vehicle_velocity * (inputs.imu_data->yaw_rate - body_slip_rate_of_change));
    const double front_axle_portion = ((body_lat_force * DIST_REAR_AXLE_CG) + inputs.desired_yawMoment) / WHEELBASE_m;
    const double rear_axle_portion  = ((body_lat_force * DIST_FRONT_AXLE_CG) - inputs.desired_yawMoment) / WHEELBASE_m;

    return (tire_forces){
        .fl = front_axle_portion / 2,
        .fr = front_axle_portion / 2,
        .rl = rear_axle_portion / 2,
        .rr = rear_axle_portion / 2,
    };
}

// Based on friction circle equation at page 59
static tire_forces app_frictionCircle_calcAvailable_Fx(tire_forces max_tractive_force, tire_forces required_Fy)
{
    return (tire_forces){
        .fl = sqrt(pow(max_tractive_force.fl, 2) - pow(required_Fy.fl, 2)),
        .fr = sqrt(pow(max_tractive_force.fr, 2) - pow(required_Fy.fr, 2)),
        .rl = sqrt(pow(max_tractive_force.rl, 2) - pow(required_Fy.rl, 2)),
        .rr = sqrt(pow(max_tractive_force.rr, 2) - pow(required_Fy.rr, 2)),
    };
}
