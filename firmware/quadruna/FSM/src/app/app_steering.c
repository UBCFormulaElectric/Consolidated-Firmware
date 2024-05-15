#include "app_steering.h"
#include <stdlib.h>
#include <assert.h>
#include "app_canTx.h"
#include "app_rangeCheck.h"
#include "app_canAlerts.h"
#include "io_steering.h"

static const RangeCheck steering_angle_in_range_check = {
    .min_value = MIN_STEERING_ANGLE_DEG,
    .max_value = MAX_STEERING_ANGLE_DEG,
};

void app_steering_broadcast(void)
{
    app_canTx_FSM_SteeringAngle_set(io_steering_getAngleDegrees());

    bool steering_sensor_ocsc = io_steering_sensorOCSC();
    app_canAlerts_FSM_Warning_SteeringAngleOCSC_set(steering_sensor_ocsc);
    if (steering_sensor_ocsc)
    {
        app_canTx_FSM_SteeringAngle_set(0);
    }

    float            steering_angle = io_steering_getAngleDegrees();
    RangeCheckStatusMetaData steering_in_range = app_rangeCheck_getValue(&steering_angle_in_range_check, steering_angle);
    app_canTx_FSM_SteeringAngle_set(steering_in_range.value);
    app_canAlerts_FSM_Warning_SteeringAngleOutOfRange_set(steering_in_range.status != VALUE_IN_RANGE);
}
