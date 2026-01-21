#include "app_dynamicsInfo.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_dynamicsInfo_datatypes.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_vd_datatypes.h"
#include "app_sbgEllipse.h"
#include "app_imu.h"
#include <math.h>

/** Responsibilities
 * slip ratio estimation
 * slip angle estimation
 * drag force estimation
 * down force estimation
 * normal force estimation
 *
 */

/************************* Private Function Prototypes *****************************/
static dynamicsEst_inputs      app_dynamicsInfo_getInputs(VD_WheelRpms wheel_rpms);
static void                    app_dynamicsInfo_broadCast(dynamicsEst_outputs out);
static inline tire_forces      app_dynamicsInfo_calcNormalForce(double lat_load_tf, double long_load_tf);
static inline double           app_dynamicsInfo_calcLatLoadTransfer(float lat_accel);
static inline double           app_dynamicsInfo_calcLongLoadTransfer(float long_accel);
static inline slip_ratios      app_dynamicsInfo_calcSlipRatio(dynamicsEst_inputs in, slip_angles slip_angles);
static inline body_slip_angles app_dynamicsInfo_calcBodySideSlip(dynamicsEst_inputs in);
// TODOs
static inline slip_angles   app_dynamicsInfo_calcSlipAngle(void);
static inline eff_rads      app_dynamicsInfo_calcEffectiveRadius(void);
static inline double        app_dynamicsInfo_calcDragForce(void);
static inline double        app_dynamicsInfo_calcDownForce(void);
static road_tire_fric_coeff app_dynamicsInfo_calcFrictionCoeff(void);

/************************* Global Function Definition *****************************/

dynamicsEst_outputs app_dynamicsInfo_estimate(VD_WheelRpms wheel_rpms)
{
    const dynamicsEst_inputs in = app_dynamicsInfo_getInputs(wheel_rpms);

    const double lat_load_tf  = app_dynamicsInfo_calcLatLoadTransfer(in.imu_data->lat_accel);
    const double long_load_tf = app_dynamicsInfo_calcLongLoadTransfer(in.imu_data->long_accel);

    const dynamicsEst_outputs out = { .lat_loadTransfer  = lat_load_tf,
                                      .long_loadTransfer = long_load_tf,
                                      .Fx_drag           = app_dynamicsInfo_calcDragForce(),
                                      .Fz_down_force     = app_dynamicsInfo_calcDownForce(),
                                      .tireModel.normal_forces =
                                          app_dynamicsInfo_calcNormalForce(lat_load_tf, long_load_tf),
                                      .tireModel.slip_angles = app_dynamicsInfo_calcSlipAngle(),
                                      .tireModel.slip_ratios = app_dynamicsInfo_calcSlipRatio(in, (slip_angles){ 0 }),
                                      .tireModel.eff_rads    = app_dynamicsInfo_calcEffectiveRadius(),
                                      .tireModel.tire_fric_coeffs = app_dynamicsInfo_calcFrictionCoeff(),
                                      .axle_body_slip             = app_dynamicsInfo_calcBodySideSlip(in) };

    // Fix slip ratios after we have slip angles
    dynamicsEst_outputs out_mutable   = out;
    out_mutable.tireModel.slip_ratios = app_dynamicsInfo_calcSlipRatio(in, out.tireModel.slip_angles);

    app_dynamicsInfo_broadCast(out_mutable);

    return out_mutable;
}

/************************* Private Function Definition *****************************/

static dynamicsEst_inputs app_dynamicsInfo_getInputs(VD_WheelRpms wheel_rpms)
{
    // vx: longitudinal velocity
    const ImuData              *imu_data       = app_imu_getData();
    vehicle_velocity_components vel_components = app_sbgEllipse_getVehicleVelocity();
    const double                vx_cg          = vel_components.vehicle_vel_vx;

    const dynamicsEst_inputs inputs = {
        .imu_data = imu_data,
        .wheel_speeds = {
            .wheel_speed_fl = MOTOR_RPM_TO_KMH(wheel_rpms.wheel_rpm_fl),
            .wheel_speed_fr = MOTOR_RPM_TO_KMH(wheel_rpms.wheel_rpm_fr),
            .wheel_speed_rl = MOTOR_RPM_TO_KMH(wheel_rpms.wheel_rpm_rl),
            .wheel_speed_rr = MOTOR_RPM_TO_KMH(wheel_rpms.wheel_rpm_rr)
        },
        // based on simple rigid body kinematics
        .wheel_center_vx = {
            .normalized_vx_fl = vx_cg - imu_data->yaw_rate * (TRACK_WIDTH_m / 2),
            .normalized_vx_fr = vx_cg + imu_data->yaw_rate * (TRACK_WIDTH_m / 2),
            .normalized_vx_rl = vx_cg - imu_data->yaw_rate * (TRACK_WIDTH_m / 2),
            .normalized_vx_rr = vx_cg + imu_data->yaw_rate * (TRACK_WIDTH_m / 2),
        },
        .vel_components = vel_components
    };
    return inputs;
}

static void app_dynamicsInfo_broadCast(dynamicsEst_outputs out)
{
    app_canTx_VC_Normal_fl_set((float)out.tireModel.normal_forces.Fz_fl);
    app_canTx_VC_Normal_fr_set((float)out.tireModel.normal_forces.Fz_fr);
    app_canTx_VC_Normal_rl_set((float)out.tireModel.normal_forces.Fz_rl);
    app_canTx_VC_Normal_rr_set((float)out.tireModel.normal_forces.Fz_rr);
    app_canTx_VC_Drag_Force_set((float)out.Fx_drag);
    app_canTx_VC_Down_Force_set((float)out.Fz_down_force);

    app_canTx_VC_SlipRatio_fl_set((float)out.tireModel.slip_ratios.fl);
    app_canTx_VC_SlipRatio_fr_set((float)out.tireModel.slip_ratios.fr);
    app_canTx_VC_SlipRatio_rl_set((float)out.tireModel.slip_ratios.rl);
    app_canTx_VC_SlipRatio_rr_set((float)out.tireModel.slip_ratios.rr);

    app_canTx_VC_SlipAngle_fl_set((float)out.tireModel.slip_angles.SA_fl);
    app_canTx_VC_SlipAngle_fr_set((float)out.tireModel.slip_angles.SA_fr);
    app_canTx_VC_SlipAngle_rl_set((float)out.tireModel.slip_angles.SA_rl);
    app_canTx_VC_SlipAngle_rr_set((float)out.tireModel.slip_angles.SA_rr);

    app_canTx_VC_EffRad_fl_m_set((float)out.tireModel.eff_rads.eff_rad_fl);
    app_canTx_VC_EffRad_fr_m_set((float)out.tireModel.eff_rads.eff_rad_fr);
    app_canTx_VC_EffRad_rl_m_set((float)out.tireModel.eff_rads.eff_rad_rl);
    app_canTx_VC_EffRad_rr_m_set((float)out.tireModel.eff_rads.eff_rad_rr);

    app_canTx_VC_Lat_load_transfer_set((float)out.lat_loadTransfer);
    app_canTx_VC_Long_load_tranfer_set((float)out.long_loadTransfer);
}

// TODO: Implement this
static road_tire_fric_coeff app_dynamicsInfo_calcFrictionCoeff(void)
{
    return (road_tire_fric_coeff){
        .fric_coeff_fl = 0.85,
        .fric_coeff_fr = 0.85,
        .fric_coeff_rl = 0.85,
        .fric_coeff_rr = 0.85,
    };
}

/************************* Helper Function Definition *****************************/
static inline tire_forces app_dynamicsInfo_calcNormalForce(double lat_load_tf, double long_load_tf)
{
    return (tire_forces){ .Fz_fl = FRONT_CONST_LOAD - lat_load_tf - long_load_tf,
                          .Fz_fr = FRONT_CONST_LOAD + lat_load_tf - long_load_tf,
                          .Fz_rl = REAR_CONST_LOAD - lat_load_tf + long_load_tf,
                          .Fz_rr = REAR_CONST_LOAD + lat_load_tf + long_load_tf };
}

static inline double app_dynamicsInfo_calcLatLoadTransfer(float lat_accel)
{
    return LAT_LOAD_TRANSFER(lat_accel) / 4.0;
}

static inline double app_dynamicsInfo_calcLongLoadTransfer(float long_accel)
{
    return LONG_LOAD_TRANSFER(long_accel) / 4.0;
}

// TODO: figure out how to calculate slip angle
static inline slip_angles app_dynamicsInfo_calcSlipAngle(void)
{
    return (slip_angles){ .SA_fl = 0.0, .SA_fr = 0.0, .SA_rl = 0.0, .SA_rr = 0.0 };
}

static inline slip_ratios app_dynamicsInfo_calcSlipRatio(dynamicsEst_inputs in, slip_angles slip_angles)
{
    return (slip_ratios){
        .fl = SLIP_RATIO(in.wheel_speeds.wheel_speed_fl, in.wheel_center_vx.normalized_vx_fl, slip_angles.SA_fl),
        .fr = SLIP_RATIO(in.wheel_speeds.wheel_speed_fr, in.wheel_center_vx.normalized_vx_fr, slip_angles.SA_fr),
        .rl = SLIP_RATIO(in.wheel_speeds.wheel_speed_rl, in.wheel_center_vx.normalized_vx_rl, slip_angles.SA_rl),
        .rr = SLIP_RATIO(in.wheel_speeds.wheel_speed_rr, in.wheel_center_vx.normalized_vx_rr, slip_angles.SA_rr)
    };
}
// TODO: figure out how to calculate and use drag and down force
static inline double app_dynamicsInfo_calcDragForce(void)
{
    return 0.0;
}

static inline double app_dynamicsInfo_calcDownForce(void)
{
    return 0.0;
}

// TODO: figure out how to find effective radius (to keep things consistent lets use empirical units so meters)
static inline eff_rads app_dynamicsInfo_calcEffectiveRadius(void)
{
    return (eff_rads){
        .eff_rad_fl = IN_TO_M * WHEEL_DIAMETER_IN / 2,
        .eff_rad_fr = IN_TO_M * WHEEL_DIAMETER_IN / 2,
        .eff_rad_rl = IN_TO_M * WHEEL_DIAMETER_IN / 2,
        .eff_rad_rr = IN_TO_M * WHEEL_DIAMETER_IN / 2,
    };
}

// EQN 2.13 in Vehicle Dynamics and Controls - Rajesh Rajamani
// TODO: adjust wheel angle calculation based on actual steering model (reverse ackermann)
static inline body_slip_angles app_dynamicsInfo_calcBodySideSlip(dynamicsEst_inputs in)
{
    return (body_slip_angles){ .front_body_slip = atan(
                                   (in.vel_components.vehicle_vel_vy + in.imu_data->yaw_rate * DIST_FRONT_AXLE_CG) /
                                   (in.vel_components.vehicle_vel_vx + 0.1)),
                               .rear_body_slip = atan(
                                   (in.vel_components.vehicle_vel_vy - in.imu_data->yaw_rate * DIST_REAR_AXLE_CG) /
                                   (in.vel_components.vehicle_vel_vx + 0.1)) };
}
