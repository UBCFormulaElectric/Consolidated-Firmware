#include "app_yawRateController.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_units.h"
#include "app_utils.h"
#include <math.h>

/*
 * NOTE:
 *
 * Untested and Untuned
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

// void app_yawRateController_computeTorque(YawRateController *yrc)
// {
//     float lambda        = 0.0f;
//     float yaw_rate_diff = fabsf(DEG_TO_RAD(yrc->ref_yaw_rate_deg)) - fabsf(DEG_TO_RAD(yrc->real_yaw_rate_deg));

//     if (yaw_rate_diff <= yrc->k4)
//     {
//         lambda = 0.0f;
//     }
//     else if (yaw_rate_diff > yrc->k4 && yaw_rate_diff < yrc->k5)
//     {
//         lambda = (yaw_rate_diff - yrc->k4) / (yrc->k5 - yrc->k4);
//     }
//     else if (yaw_rate_diff >= yrc->k5)
//     {
//         lambda = 1.0f;
//     }

//     yrc->torque_fl_Nm = (0.25f * yrc->requested_torque) + 0.5f * (1 - lambda) * yrc->yaw_moment;
//     yrc->torque_fr_Nm = (0.25f * yrc->requested_torque) - 0.5f * (1 - lambda) * yrc->yaw_moment;
//     yrc->torque_rl_Nm = (0.25f * yrc->requested_torque) + 0.5f * lambda * yrc->yaw_moment;
//     yrc->torque_rr_Nm = (0.25f * yrc->requested_torque) - 0.5f * lambda * yrc->yaw_moment;
// }