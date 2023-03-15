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
    struct Wheels *wheels = App_FsmWorld_GetWheels(world);

    struct InRangeCheck *left_wheel_speed_in_range_check  = App_Wheels_GetLeftWheelSpeedInRange(wheels);
    struct InRangeCheck *right_wheel_speed_in_range_check = App_Wheels_GetRightWheelSpeedInRange(wheels);

    App_SetPeriodicCanSignals_InRangeCheck_float(
        left_wheel_speed_in_range_check, App_CanTx_FSM_Wheels_LeftWheelSpeed_Set,
        (void (*)(uint8_t))App_CanTx_FSM_Warnings_LeftWheelSpeedOutOfRange_Set);

    App_SetPeriodicCanSignals_InRangeCheck_float(
        right_wheel_speed_in_range_check, App_CanTx_FSM_Wheels_RightWheelSpeed_Set,
        (void (*)(uint8_t))App_CanTx_FSM_Warnings_RightWheelSpeedOutOfRange_Set);
}
