#include <stdlib.h>
#include <assert.h>
#include "App_Wheels.h"
#include "configs/App_WheelSpeedThresholds.h"

#include "App_InRangeCheck.h"
#include "App_SharedSetPeriodicCanSignals.h"

struct Wheels
{
    float (*get_left_wheel_speed)(void);
    float (*get_right_wheel_speed)(void);
    struct InRangeCheck *left_wheel_speed_in_range_check;
    struct InRangeCheck *right_wheel_speed_in_range_check;
};

struct Wheels *App_Wheels_Create(float (*get_left_wheel_speed)(void), float (*get_right_wheel_speed)(void))
{
    struct Wheels *wheels = malloc(sizeof(struct Wheels));
    assert(wheels != NULL);

    wheels->get_left_wheel_speed  = get_left_wheel_speed;
    wheels->get_right_wheel_speed = get_right_wheel_speed;

    wheels->left_wheel_speed_in_range_check =
        App_InRangeCheck_Create(get_left_wheel_speed, MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH);
    wheels->right_wheel_speed_in_range_check =
        App_InRangeCheck_Create(get_right_wheel_speed, MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH);

    return wheels;
}
void App_Wheels_Destroy(struct Wheels *wheels)
{
    App_InRangeCheck_Destroy(wheels->left_wheel_speed_in_range_check);
    App_InRangeCheck_Destroy(wheels->right_wheel_speed_in_range_check);
    free(wheels);
}

struct InRangeCheck *App_Wheels_GetLeftWheelSpeedInRange(const struct Wheels *wheels)
{
    return wheels->left_wheel_speed_in_range_check;
}

struct InRangeCheck *App_Wheels_GetRightWheelSpeedInRange(const struct Wheels *wheels)
{
    return wheels->right_wheel_speed_in_range_check;
}

void App_Wheels_Broadcast(const struct FsmWorld *world)
{
    struct Wheels *      wheels                           = App_FsmWorld_GetWheels(world);
    struct InRangeCheck *left_wheel_speed_in_range_check  = App_Wheels_GetLeftWheelSpeedInRange(wheels);
    struct InRangeCheck *right_wheel_speed_in_range_check = App_Wheels_GetRightWheelSpeedInRange(wheels);

    float                    left_wheel_speed;
    enum InRangeCheck_Status left_wheel_status =
        App_InRangeCheck_GetValue(left_wheel_speed_in_range_check, &left_wheel_speed);
    App_CanTx_FSM_LeftWheelSpeed_Set(left_wheel_speed);
    App_CanAlerts_SetWarning(FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE, left_wheel_status != VALUE_IN_RANGE);

    float                    right_wheel_speed;
    enum InRangeCheck_Status right_wheel_status =
        App_InRangeCheck_GetValue(right_wheel_speed_in_range_check, &right_wheel_speed);
    App_CanTx_FSM_RightWheelSpeed_Set(right_wheel_speed);
    App_CanAlerts_SetWarning(FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE, right_wheel_status != VALUE_IN_RANGE);
}
