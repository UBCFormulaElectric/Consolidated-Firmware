#include "app_wheels.h"
#include <stdlib.h>
#include <assert.h>
#include "app_rangeCheck.h"
#include "App_CanTx.h"
#include "App_CanAlerts.h"
#include "io_wheels.h"

static const RangeCheck left_wheel_speed_in_range_check = {
    .get_value = io_wheels_getLeftSpeedKph,
    .min_value = MIN_LEFT_WHEEL_SPEED_KPH,
    .max_value = MAX_LEFT_WHEEL_SPEED_KPH,
};
static const RangeCheck right_wheel_speed_in_range_check = {
    .get_value = io_wheels_getRightSpeedKph,
    .min_value = MIN_RIGHT_WHEEL_SPEED_KPH,
    .max_value = MAX_RIGHT_WHEEL_SPEED_KPH,
};

void app_wheels_broadcast()
{
    float            left_wheel_speed;
    RangeCheckStatus left_wheel_status = app_rangeCheck_getValue(&left_wheel_speed_in_range_check, &left_wheel_speed);
    App_CanTx_FSM_LeftWheelSpeed_Set(left_wheel_speed);
    App_CanAlerts_FSM_Warning_LeftWheelSpeedOutOfRange_Set(left_wheel_status != VALUE_IN_RANGE);

    float            right_wheel_speed;
    RangeCheckStatus right_wheel_status =
        app_rangeCheck_getValue(&right_wheel_speed_in_range_check, &right_wheel_speed);
    App_CanTx_FSM_RightWheelSpeed_Set(right_wheel_speed);
    App_CanAlerts_FSM_Warning_RightWheelSpeedOutOfRange_Set(right_wheel_status != VALUE_IN_RANGE);
}
