#include "app_wheels.h"
#include <stdlib.h>
#include <assert.h>
#include "App_InRangeCheck.h"
#include "App_CanTx.h"
#include "App_CanAlerts.h"
#include "io_wheels.h"

static struct InRangeCheck *left_wheel_speed_in_range_check;
static struct InRangeCheck *right_wheel_speed_in_range_check;

void app_wheels_init()
{
    left_wheel_speed_in_range_check =
        App_InRangeCheck_Create(io_wheels_getLeftSpeedKph, MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH);
    right_wheel_speed_in_range_check =
        App_InRangeCheck_Create(io_wheels_getRightSpeedKph, MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH);
}

void app_wheels_destroy()
{
    App_InRangeCheck_Destroy(left_wheel_speed_in_range_check);
    App_InRangeCheck_Destroy(right_wheel_speed_in_range_check);
}

void app_wheels_broadcast()
{
    float                    left_wheel_speed;
    enum InRangeCheck_Status left_wheel_status =
        App_InRangeCheck_GetValue(left_wheel_speed_in_range_check, &left_wheel_speed);
    App_CanTx_FSM_LeftWheelSpeed_Set(left_wheel_speed);
    App_CanAlerts_FSM_Warning_LeftWheelSpeedOutOfRange_Set(left_wheel_status != VALUE_IN_RANGE);

    float                    right_wheel_speed;
    enum InRangeCheck_Status right_wheel_status =
        App_InRangeCheck_GetValue(right_wheel_speed_in_range_check, &right_wheel_speed);
    App_CanTx_FSM_RightWheelSpeed_Set(right_wheel_speed);
    App_CanAlerts_FSM_Warning_RightWheelSpeedOutOfRange_Set(right_wheel_status != VALUE_IN_RANGE);
}
