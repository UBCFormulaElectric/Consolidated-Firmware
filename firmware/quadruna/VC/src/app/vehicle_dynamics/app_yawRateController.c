#include "app_yawRateController.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_units.h"
#include "utils.h"
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