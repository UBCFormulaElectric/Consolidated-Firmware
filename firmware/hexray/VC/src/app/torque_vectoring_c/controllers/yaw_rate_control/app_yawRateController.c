#include "app_yawRateController.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_units.h"
#include "app_utils.h"
#include "../../dynamics_estimation/app_frictionCircle_datatypes.h"
#include "../../dynamics_estimation/app_dynamicsInfo_datatypes.h"
#include "app_imu.h"
#include "app_sbgEllipse.h"
#include <math.h>

/*
 * Current tuning constants are stable
 *
 * kp = 0.25, ki = 0.0, kd = 0.0
 * ku = 0.0
 */

static float ref_yaw_rate_rad;
static float yaw_moment;

void app_yawRateController_init(YawRateController *yrc, PID *pid, const YawRateController_Config *config)
{
    app_pid_init(pid, config->pid_config);

    yrc->pid                  = pid;
    yrc->ku                   = config->ku;
    yrc->wheel_angle_rad      = 0.0f;
    yrc->vehicle_velocity_mps = 0.0f;
    yrc->real_yaw_rate_rad    = 0.0f;
    ref_yaw_rate_rad          = 0.0f;
    yaw_moment                = 0.0f;
}

void app_yawRateController_computeRefYawRate(YawRateController *yrc)
{
    /*
     * Formula for reference yaw rate:
     *
     * r_ref = (vehicle_velocity * wheel_angle) / (wheel_base + ku * vehicle_velocity^2)
     */
    // TODO: add clamp in here to prevent steering out of range being an issue
    ref_yaw_rate_rad = (yrc->vehicle_velocity_mps * yrc->wheel_angle_rad) /
                       ((WHEELBASE_mm * MM_TO_M) + (yrc->ku * SQUARE(yrc->vehicle_velocity_mps)));
}

void app_yawRateController_pidCompute(YawRateController *yrc)
{
    yaw_moment = app_pid_compute(yrc->pid, ref_yaw_rate_rad, yrc->real_yaw_rate_rad);
}

void app_yawRateController_run(YawRateController *yrc)
{
    app_yawRateController_computeRefYawRate(yrc);
    app_yawRateController_pidCompute(yrc);
}

float app_yawRateController_getRefYawRateRad()
{
    return ref_yaw_rate_rad;
}

float app_yawRateController_getYawMoment()
{
    return yaw_moment;
}

// TODO: Implement properly - this is a stub
VD_frictionCircle_inputs app_yawRateController_getFrictionCircleInputs(void)
{
    const ImuData              *imu_data_ptr = app_imu_getData();
    ImuData                     imu_data     = *imu_data_ptr;
    vehicle_velocity_components vel          = app_sbgEllipse_getVehicleVelocity();

    return (
        VD_frictionCircle_inputs){ .imu_data           = imu_data,
                                   .desired_yawRate    = (double)ref_yaw_rate_rad,
                                   .desired_yawMoment  = (double)yaw_moment,
                                   .vehicle_velocity   = vel.vehicle_vel,
                                   .body_slip          = 0.0,
                                   .tire_fric_coeffs   = { .fric_coeff_fl = 0.85,
                                                           .fric_coeff_fr = 0.85,
                                                           .fric_coeff_rl = 0.85,
                                                           .fric_coeff_rr = 0.85 },
                                   .tire_normal_forces = { .Fz_fl = 0.0, .Fz_fr = 0.0, .Fz_rl = 0.0, .Fz_rr = 0.0 } };
}