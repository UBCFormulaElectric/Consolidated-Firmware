#include "app_steering.h"
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
    // TODO: send can message with steering angle -> io_steering_getAngleDegrees()

    bool steering_sensor_ocsc = io_steering_sensorOCSC();
    // TODO: send can message that sends steering sensor ocsc state ^^

    if (steering_sensor_ocsc)
    {
        // TODO: send can message setting the steering angle to be 0
    }

    float                    steering_angle = io_steering_getAngleDegrees();
    RangeCheckStatusMetaData steering_in_range =
        app_rangeCheck_getValue(&steering_angle_in_range_check, steering_angle);
    // TODO: send can message with steering angle
    // TODO: send can alert if steering angle out of range -> steering_in_range.status != VALUE_IN_RANGE
}