#include "app/steering.h"

#include "app_canTx.h"
#include "app/rangeCheck.h"
#include "app_canAlerts.h"
#include "io/steering.h"

static const RangeCheck steering_angle_in_range_check = {
    .min_value = MIN_STEERING_ANGLE_DEG,
    .max_value = MAX_STEERING_ANGLE_DEG,
};

void app_steering_broadcast(void)
{
    const float steering_angle       = io_steering_getAngleDegrees();
    const bool  steering_sensor_ocsc = io_steering_sensorOCSC();
    app_canTx_FSM_SteeringAngle_set(steering_angle);
    app_canAlerts_FSM_Info_SteeringAngleOCSC_set(steering_sensor_ocsc);
    const RangeCheckStatusMetaData steering_in_range =
        app_rangeCheck_getValue(&steering_angle_in_range_check, steering_angle);
    app_canTx_FSM_SteeringAngle_set(steering_angle);
    app_canAlerts_FSM_Info_SteeringAngleOutOfRange_set(steering_in_range.status != VALUE_IN_RANGE);
}