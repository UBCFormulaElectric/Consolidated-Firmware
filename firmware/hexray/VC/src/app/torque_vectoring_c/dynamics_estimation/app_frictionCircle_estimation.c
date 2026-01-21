#include "app_frictionCircle_estimation.h"
#include "app_dynamicsInfo_datatypes.h"
#include "app_frictionCircle_datatypes.h"
#include "app_yawRateController.h"
#include "math.h"

#define TASK_RATE ((double)(10.0 / 1000)) // 10ms task

/************************* Private Function Prototype *****************************/

static inline tire_forces
    app_frictionCircle_calcMaxTractiveForce(road_tire_fric_coeff friction_coeffs, tire_forces normal_forces);
static tire_forces        app_frictionCircle_calcRequired_Fy(VD_frictionCircle_inputs inputs);
static inline tire_forces app_frictionCircle_calcAvailable_Fx(tire_forces max_tractive_force, tire_forces required_Fy);

/************************* Global Function Definition *****************************/

VD_frictionCircle_outputs app_frictionCircle_estimate(void)
{
    const VD_frictionCircle_inputs inputs = app_yawRateController_getFrictionCircleInputs();

    const tire_forces max_tractive_forces =
        app_frictionCircle_calcMaxTractiveForce(inputs.tire_fric_coeffs, inputs.tire_normal_forces);
    const tire_forces required_Fy = app_frictionCircle_calcRequired_Fy(inputs);

    return (VD_frictionCircle_outputs){ .maximum_tractive_forces = max_tractive_forces,
                                        .required_Fy             = required_Fy,
                                        .avail_Fx =
                                            app_frictionCircle_calcAvailable_Fx(max_tractive_forces, required_Fy) };
}

/************************* Static Function Definition *****************************/
static VD_frictionCircle_inputs __attribute__((unused)) app_frictionCircle_inputs(
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

static void __attribute__((unused)) app_frictionCircle_broadcast(VD_frictionCircle_outputs out)
{
    app_canTx_VC_FrictionCircle_MaxTractiveForce_fl_set((float)out.maximum_tractive_forces.Fz_fl);
    app_canTx_VC_FrictionCircle_MaxTractiveForce_fr_set((float)out.maximum_tractive_forces.Fz_fr);
    app_canTx_VC_FrictionCircle_MaxTractiveForce_rl_set((float)out.maximum_tractive_forces.Fz_rl);
    app_canTx_VC_FrictionCircle_MaxTractiveForce_rr_set((float)out.maximum_tractive_forces.Fz_rr);

    app_canTx_VC_FrictionCircle_Required_Fy_fl_set((float)out.required_Fy.Fz_fl);
    app_canTx_VC_FrictionCircle_Required_Fy_fr_set((float)out.required_Fy.Fz_fr);
    app_canTx_VC_FrictionCircle_Required_Fy_rl_set((float)out.required_Fy.Fz_rl);
    app_canTx_VC_FrictionCircle_Required_Fy_rr_set((float)out.required_Fy.Fz_rr);

    app_canTx_VC_FrictionCircle_Avail_Fx_fl_set((float)out.avail_Fx.Fz_fl);
    app_canTx_VC_FrictionCircle_Avail_Fx_fr_set((float)out.avail_Fx.Fz_fr);
    app_canTx_VC_FrictionCircle_Avail_Fx_rl_set((float)out.avail_Fx.Fz_rl);
    app_canTx_VC_FrictionCircle_Avail_Fx_rr_set((float)out.avail_Fx.Fz_rr);
}

static inline tire_forces
    app_frictionCircle_calcMaxTractiveForce(road_tire_fric_coeff friction_coeffs, tire_forces normal_forces)
{
    return (tire_forces){
        .Fz_fl = friction_coeffs.fric_coeff_fl * normal_forces.Fz_fl,
        .Fz_fr = friction_coeffs.fric_coeff_fr * normal_forces.Fz_fr,
        .Fz_rl = friction_coeffs.fric_coeff_rl * normal_forces.Fz_rl,
        .Fz_rr = friction_coeffs.fric_coeff_rr * normal_forces.Fz_rr,
    };
}
// TODO: modify to accomodate load transfer
static tire_forces app_frictionCircle_calcRequired_Fy(VD_frictionCircle_inputs inputs)
{
    static double prev_body_slip           = 0.0;
    const double  body_slip_rate_of_change = (inputs.body_slip - prev_body_slip) / TASK_RATE;
    const double  body_lat_force =
        (CAR_MASS_AT_CG_KG * inputs.vehicle_velocity * (inputs.imu_data.yaw_rate - body_slip_rate_of_change));
    const double front_axle_portion = ((body_lat_force * DIST_REAR_AXLE_CG) + inputs.desired_yawMoment) / WHEELBASE_m;
    const double rear_axle_portion  = ((body_lat_force * DIST_FRONT_AXLE_CG) - inputs.desired_yawMoment) / WHEELBASE_m;
    prev_body_slip                  = inputs.body_slip;

    return (tire_forces){
        .Fz_fl = front_axle_portion / 2,
        .Fz_fr = front_axle_portion / 2,
        .Fz_rl = rear_axle_portion / 2,
        .Fz_rr = rear_axle_portion / 2,
    };
}

// Based on friction circle equation at page 59
static tire_forces app_frictionCircle_calcAvailable_Fx(tire_forces max_tractive_force, tire_forces required_Fy)
{
    return (tire_forces){
        .Fz_fl = sqrt(pow(max_tractive_force.Fz_fl, 2) - pow(required_Fy.Fz_fl, 2)),
        .Fz_fr = sqrt(pow(max_tractive_force.Fz_fr, 2) - pow(required_Fy.Fz_fr, 2)),
        .Fz_rl = sqrt(pow(max_tractive_force.Fz_rl, 2) - pow(required_Fy.Fz_rl, 2)),
        .Fz_rr = sqrt(pow(max_tractive_force.Fz_rr, 2) - pow(required_Fy.Fz_rr, 2)),
    };
}
