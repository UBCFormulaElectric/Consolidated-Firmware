#include "app_yawRateController.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_units.h"
#include <math.h>

/*
 * NOTE:
 *
 * Untested and Untuned
 * Currently does not sent torque requests
 */

void app_yawRateController_init(YawRateController *yrc, PID *pid, const YawRateController_Config *config)
{
    app_pid_init(pid, config->pid_config);

    yrc->pid                  = pid;
    yrc->ku                   = config->ku;
    yrc->k4                   = config->k4;
    yrc->k5                   = config->k5;
    yrc->requested_torque     = 0.0f;
    yrc->wheel_angle_deg      = 0.0f;
    yrc->vehicle_velocity_kmh = 0.0f;
    yrc->real_yaw_rate_deg    = 0.0f;
    yrc->ref_yaw_rate_deg     = 0.0f;
    yrc->yaw_moment           = 0.0f;
    yrc->torque_fl_Nm         = 0.0f;
    yrc->torque_fr_Nm         = 0.0f;
    yrc->torque_rl_Nm         = 0.0f;
    yrc->torque_rr_Nm         = 0.0f;
}

void app_yawRateController_computeRefYawRate(YawRateController *yrc)
{
    float ref_yaw_rate_rad =
        (KMH_TO_MPS(yrc->vehicle_velocity_kmh) * DEG_TO_RAD(yrc->wheel_angle_deg * APPROX_STEERING_TO_WHEEL_ANGLE)) /
        ((WHEELBASE_mm * MM_TO_M) +
         (yrc->ku * (KMH_TO_MPS(yrc->vehicle_velocity_kmh)) * (KMH_TO_MPS(yrc->vehicle_velocity_kmh))));
    yrc->ref_yaw_rate_deg = RAD_TO_DEG(ref_yaw_rate_rad);
}

void app_yawRateController_pidCompute(YawRateController *yrc)
{
    yrc->yaw_moment = app_pid_compute(yrc->pid, DEG_TO_RAD(yrc->ref_yaw_rate_deg), DEG_TO_RAD(yrc->real_yaw_rate_deg));
}

void app_yawRateController_computeTorque(YawRateController *yrc)
{
    float lambda        = 0.0f;
    float yaw_rate_diff = fabsf(DEG_TO_RAD(yrc->ref_yaw_rate_deg)) - fabsf(DEG_TO_RAD(yrc->real_yaw_rate_deg));

    if (yaw_rate_diff <= yrc->k4)
    {
        lambda = 0.0f;
    }
    else if (yaw_rate_diff > yrc->k4 && yaw_rate_diff < yrc->k5)
    {
        lambda = (yaw_rate_diff - yrc->k4) / (yrc->k5 - yrc->k4);
    }
    else if (yaw_rate_diff >= yrc->k5)
    {
        lambda = 1.0f;
    }

    yrc->torque_fl_Nm = (0.25f * yrc->requested_torque) + 0.5f * (1 - lambda) * yrc->yaw_moment;
    yrc->torque_fr_Nm = (0.25f * yrc->requested_torque) - 0.5f * (1 - lambda) * yrc->yaw_moment;
    yrc->torque_rl_Nm = (0.25f * yrc->requested_torque) + 0.5f * lambda * yrc->yaw_moment;
    yrc->torque_rr_Nm = (0.25f * yrc->requested_torque) - 0.5f * lambda * yrc->yaw_moment;
}