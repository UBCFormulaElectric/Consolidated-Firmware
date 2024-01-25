#include "app_steering.h"
#include <stdlib.h>
#include <assert.h>
#include "App_InRangeCheck.h"
#include "App_CanTx.h"
#include "App_CanAlerts.h"
#include "io_steering.h"

static struct InRangeCheck* steering_angle_in_range_check;

void app_steering_init(void)
{
    steering_angle_in_range_check =
        App_InRangeCheck_Create(io_steering_getAngleDegrees, MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG);
}
void app_steering_destroy(void)
{
    App_InRangeCheck_Destroy(steering_angle_in_range_check);
}

void app_steering_broadcast(void)
{
    App_CanTx_FSM_SteeringAngle_Set(io_steering_getAngleDegrees());

    bool steering_sensor_ocsc = io_steering_sensorOCSC();
    App_CanAlerts_FSM_Warning_SteeringAngleOCSC_Set(steering_sensor_ocsc);
    if (steering_sensor_ocsc)
    {
        App_CanTx_FSM_SteeringAngle_Set(0);
    }

    float                    steering_angle;
    enum InRangeCheck_Status steering_in_range =
        App_InRangeCheck_GetValue(steering_angle_in_range_check, &steering_angle);
    App_CanTx_FSM_SteeringAngle_Set(steering_angle);
    App_CanAlerts_FSM_Warning_SteeringAngleOutOfRange_Set(steering_in_range != VALUE_IN_RANGE);
}
