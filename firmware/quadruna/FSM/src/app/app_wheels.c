#include "app_wheels.h"
#include "app_rangeCheck.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_wheels.h"

static const RangeCheck left_wheel_speed_in_range_check = {
    .min_value = MIN_LEFT_WHEEL_SPEED_KPH,
    .max_value = MAX_LEFT_WHEEL_SPEED_KPH,
};
static const RangeCheck right_wheel_speed_in_range_check = {
    .min_value = MIN_RIGHT_WHEEL_SPEED_KPH,
    .max_value = MAX_RIGHT_WHEEL_SPEED_KPH,
};

void app_wheels_broadcast(void)
{
    float                    left_wheel_speed = io_wheels_getLeftSpeedKph();
    RangeCheckStatusMetaData left_wheel_status =
        app_rangeCheck_getValue(&left_wheel_speed_in_range_check, left_wheel_speed);
    app_canTx_FSM_LeftWheelSpeed_set(left_wheel_speed);
    app_canAlerts_FSM_Warning_LeftWheelSpeedOutOfRange_set(left_wheel_status.status != VALUE_IN_RANGE);

    float                    right_wheel_speed = io_wheels_getRightSpeedKph();
    RangeCheckStatusMetaData right_wheel_status =
        app_rangeCheck_getValue(&right_wheel_speed_in_range_check, right_wheel_speed);
    app_canTx_FSM_RightWheelSpeed_set(right_wheel_speed);
    app_canAlerts_FSM_Warning_RightWheelSpeedOutOfRange_set(right_wheel_status.status != VALUE_IN_RANGE);
}
