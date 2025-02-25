#include "app_yawRateController.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_units.h"
#include <math.h>

/*
 * NOTE:
 *  
 * Untested and Untuned
 * Currently does not sent torque requests
 * Currently adapted for 2 wheel driving,
*/

void app_yawRateController_init(YawRateController *yrc, PID *pid, const YawRateController_Config *config)
{
    app_pid_init(pid, config->pid_config);

    yrc->pid              = pid;
    yrc->requested_torque = 0.0f;
    yrc->wheel_angle_deg  = 0.0f;
    yrc->vehicle_velocity = 0.0f;
    yrc->real_yaw_rate    = 0.0f;
    yrc->ku               = config->ku;
    yrc->k4               = config->k4;
    yrc->k5               = config->k5;
    yrc->torque_left_Nm   = 0.0f;
    yrc->torque_right_Nm  = 0.0f;
}

float app_yawRateController_computeRefYawRate(YawRateController *yrc)
{
    return (yrc->vehicle_velocity * yrc->wheel_angle_deg) /
           ((WHEELBASE_mm * MM_TO_M) + (yrc->ku * yrc->vehicle_velocity * yrc->vehicle_velocity));
}

float app_yawRateController_pid_compute(YawRateController *yrc, float ref_yaw_rate)
{
    return app_pid_compute(yrc->pid, ref_yaw_rate, yrc->real_yaw_rate);
}

void app_yawRateController_computeTorque(
    YawRateController *yrc,
    float              ref_yaw_rate,
    float              yaw_moment)
{
    float lambda;
    float yaw_rate_diff = fabsf(ref_yaw_rate) - fabsf(yrc->real_yaw_rate);

    if (yaw_rate_diff <= yrc->k4) {
        lambda = 0;
    } else if (yaw_rate_diff > yrc->k4 && yaw_rate_diff < yrc->k5) {
        lambda = fabsf(yrc->real_yaw_rate) - yrc->k4) / (yrc->k5 - yrc->k4);
    } else if (yaw_rate_diff >= yrc->k5) {
        lambda = 1;
    }

    yrc->torque_left_Nm  = (0.5f * yrc->requested_torque) + 0.5f * lambda * yaw_moment;
    yrc->torque_right_Nm = (0.5f * yrc->requested_torque) - 0.5f * lambda * yaw_moment;
}