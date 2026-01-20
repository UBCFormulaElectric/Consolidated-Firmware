#include "app_dynamicsInfo.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_dynamicsInfo_datatypes.h"
#include "app_vd_constants.h"
#include "app_vd_datatypes.h"
#include "app_sbgEllipse.h"

/** Responsibilities
 * slip ratio estimation
 * slip angle estimation
 * drag force estimation
 * down force estimation
 * normal force estimation
 *
 */

/************************* Private Function Prototypes *****************************/
static dynamicsEst_inputs      app_dynamicsInfo_getInputs(void);
static void                    app_dynamicsInfo_broadCast(dynamicsEst_outputs out);
static inline normal_forces    app_dynamicsInfo_calcNormalForce(double lat_load_tf, double long_load_tf);
static inline double           app_dynamicsInfo_calcLatLoadTransfer(float lat_accel);
static inline double           app_dynamicsInfo_calcLongLoadTransfer(float long_accel);
static inline slip_ratios      app_dynamicsInfo_calcSlipRatio(dynamicsEst_inputs in, slip_angles slip_angles);
static inline body_slip_angles app_dynamicsInfo_calcBodySideSlip(dynamicsEst_inputs in);
// TODOs
static inline slip_angles   app_dynamicsInfo_calcSlipAngle(void);
static inline eff_rads      app_dynamicsInfo_calcEffectiveRadius(void);
static inline double        app_dynamicsInfo_calcDragForce(void);
static inline double        app_dynamicsInfo_calcDownForce(void);
static road_tire_fric_coeff app_dynamicsInfo_calcFrictionCoeff();

/************************* Global Function Definition *****************************/

dynamicsEst_outputs app_dynamicsInfo_estimate(VD_WheelRpms wheel_rpms)
{
    const dynamicsEst_inputs in = app_dynamicsInfo_getInputs(wheel_rpms);

    const dynamicsEst_outputs out = {
        .lat_loadTransfer           = app_dynamicsInfo_calcLatLoadTransfer(in.imu_data->lat_accel),
        .long_loadTransfer          = app_dynamicsInfo_calcLongLoadTransfer(in.imu_data->long_accel),
        .Fx_drag                    = app_dynamicsInfo_calcDragForce(),
        .Fz_down_force              = app_dynamicsInfo_calcDownForce(),
        .tireModel.normal_forces    = app_dynamicsInfo_calcNormalForce(out.lat_loadTransfer, out.long_loadTransfer),
        .tireModel.slip_angles      = app_dynamicsInfo_calcSlipAngle(),
        .tireModel.slip_ratios      = app_dynamicsInfo_calcSlipRatio(in, out.tireModel.slip_angles),
        .tireModel.eff_rads         = app_dynamicsInfo_calcEffectiveRadius(),
        .tireModel.tire_fric_coeffs = app_dynamicsInfo_calcFrictionCoeff(),
        .axle_body_slip             = app_dynamicsInfo_calcBodySideSlip(in)
    };
    app_dynamicsInfo_broadCast(out);

    return out;
}

/************************* Private Function Definition *****************************/

static dynamicsEst_inputs app_dynamicsInfo_getInputs(VD_WheelRpms wheel_rpms, ImuData imu_data)
{
    // vx: longitudinal velocity

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
            .normalized_vx_fl = vx_cg - inputs.imu_data.yaw_rate * (TRACK_WIDTH_m / 2),
            .normalized_vx_fr = vx_cg + inputs.imu_data.yaw_rate * (TRACK_WIDTH_m / 2),
            .normalized_vx_rl = vx_cg - inputs.imu_data.yaw_rate * (TRACK_WIDTH_m / 2),
            .normalized_vx_rr = vx_cg + inputs.imu_data.yaw_rate * (TRACK_WIDTH_m / 2),
        },
        .vehicle_velocity = app_sbgEllipse_getVehicleVelocity(),
        
    };
    return inputs;
}

static void app_dynamicsInfo_broadCast(dynamicsEst_outputs out)
{
    app_canTx_VC_Normal_fl_set(out.tireModel.normal_forces.fl);
    app_canTx_VC_Normal_fr_set(out.tireModel.normal_forces.fr);
    app_canTx_VC_Normal_rl_set(out.tireModel.normal_forces.rl);
    app_canTx_VC_Normal_rr_set(out.tireModel.normal_forces.rr);
    app_canTx_VC_Drag_Force_set(out.Fx_drag);
    app_canTx_VC_Down_Force_set(out.Fz_down_force);

    app_canTx_VC_SlipRatio_fl(out.tireModel.slip_ratios.SR_fl);
    app_canTx_VC_SlipRatio_fr(out.tireModel.slip_ratios.SR_fr);
    app_canTx_VC_SlipRatio_rl(out.tireModel.slip_ratios.SR_fr);
    app_canTx_VC_SlipRatio_rr(out.tireModel.slip_ratios.SR_rr);

    app_canTx_VC_SlipAngle_fl(out.tireModel.slip_ratios.SR_fl);
    app_canTx_VC_SlipAngle_fr(out.tireModel.slip_ratios.SR_fr);
    app_canTx_VC_SlipAngle_rl(out.tireModel.slip_ratios.SR_fr);
    app_canTx_VC_SlipAngle_rr(out.tireModel.slip_ratios.SR_rr);

    app_canTx_VC_EffRad_fl_m(out.tireModel.eff_rads_eff_rad_fl);
    app_canTx_VC_EffRad_fr_m(out.tireModel.eff_rads_eff_rad_fr);
    app_canTx_VC_EffRad_rl_m(out.tireModel.eff_rads_eff_rad_rl);
    app_canTx_VC_EffRad_rr_m(out.tireModel.eff_rads_eff_rad_rr);

    app_canTx_VC_Lat_load_transfer(out.lat_loadTransfer);
    app_canTx_VC_Long_load_transfer(out.long_loadTransfer);
}

// TODO: Implement this
static road_tire_fric_coeff app_dynamicsInfo_calcFrictionCoeff()
{
    return (road_tire_fric_coeff)
    {
        .fric_coeff_fl = 0.85, .fric_coeff_fr = 0.85, .fric_coeff_rl = 0.85, .fric_coeff_rr = 0.85,
    }
}

/************************* Helper Function Definition *****************************/
static inline normal_forces app_dynamicsInfo_calcNormalForce(double lat_load_tf, double long_load_tf)
{
    return (normal_forces){ .fl = FRONT_CONST_LOAD - lat_load_tf - long_load_tf,
                            .fr = RONT_CONST_LOAD + lat_load_tf - long_load_tf,
                            .rl = REAR_CONST_LOAD - lat_load_tf + long_load_tf,
                            .rr = REAR_CONST_LOAD + lat_load_tf + long_load_tf };
}

static inline double app_dynamicsInfo_calcLatLoadTransfer(float lat_accel)
{
    return LAT_LOAD_TRANSFER(inputs.imu_data->lat_accel) / 4.0;
}

static inline double app_dynamicsInfo_calcLongLoadTransfer(void)
{
    return LONG_LOAD_TRANSFER(inputs.imu_data->long_accel) / 4.0;
}

// TODO: figure out how to calculate slip angle
static inline slip_angles app_dynamicsInfo_calcSlipAngle(void)
{
    return (slip_angles){ .SA_fl = 0.0, .SA_fr = 0.0, .SA_rl = 0.0, .SA_rr = 0.0 };
}

static inline slip_ratios app_dynamicsInfo_calcSlipRatio(dynamicsEst_inputs in, slip_angles slip_angles)
{
    return (slip_ratios){ .SR_fl = SLIP_RATIO(
                              in.wheel_speeds.wheel_speed_fl, in.wheel_center_vx.normalized_vx_fl, in.imu_data.yaw_rate,
                              slip_angles.SA_fl),
                          .SR_fr = SLIP_RATIO(
                              in.wheel_speeds.wheel_speed_fr, in.wheel_center_vx.normalized_vx_fr, in.imu_data.yaw_rate,
                              slip_angles.SA_fr),
                          .SR_rl = SLIP_RATIO(
                              in.wheel_speeds.wheel_speed_rl, in.wheel_center_vx.normalized_vx_rl, in.imu_data.yaw_rate,
                              slip_angles.SA_rl),
                          .SR_rr = SLIP_RATIO(
                              in.wheel_speeds.wheel_speed_rr, in.wheel_center_vx.normalized_vx_rr, in.imu_data.yaw_rate,
                              slip_angles.SA_rr) };
}
// TODO: figure out how to calculate and use drag and down force
static inline double app_dynamicsInfo_calcDragForce(void)
{
    return 0.0
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
    return (body_slip_angles)
    {
        .front_body_slip =
            arctan((in.vel_components.vy + in.imu_data->yaw_rate * DIST_FRONT_AXLE_CG) / (in.vel_components.vx + 0.1)),
        .rear_body_slip =
            arctan((in.vel_components.vy - in.imu_data->yaw_rate * DIST_REAR_AXLE_CG) / (in.vel_components.vx + 0.1));
    };
}
