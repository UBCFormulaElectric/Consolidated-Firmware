#include <stdlib.h>
#include <assert.h>
#include "App_Wheels.h"
#include "configs/App_WheelSpeedThresholds.h"

#include "App_InRangeCheck.h"
#include "App_SharedSetPeriodicCanSignals.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

struct Wheels{
    float (*get_left_wheel_speed)(void);
    float (*get_right_wheel_speed)(void);
    struct InRangeCheck * left_wheel_speed_in_range_check;
    struct InRangeCheck * right_wheel_speed_in_range_check;
};

struct Wheels * App_Wheels_Create(
    float (*get_left_wheel_speed)(void),
    float (*get_right_wheel_speed)(void)
){
    struct Wheels * wheels = malloc(sizeof(struct Wheels));
    assert(wheels != NULL);

    wheels->get_left_wheel_speed = get_left_wheel_speed;
    wheels->get_right_wheel_speed = get_right_wheel_speed;

    wheels->left_wheel_speed_in_range_check = App_InRangeCheck_Create(get_left_wheel_speed, MIN_LEFT_WHEEL_SPEED_KPH, MAX_LEFT_WHEEL_SPEED_KPH);
    wheels->right_wheel_speed_in_range_check = App_InRangeCheck_Create(get_right_wheel_speed, MIN_RIGHT_WHEEL_SPEED_KPH, MAX_RIGHT_WHEEL_SPEED_KPH);

    return wheels;
}

struct InRangeCheck * App_Wheels_GetLeftWheelSpeedInRange(const struct Wheels * wheels){
    return wheels->left_wheel_speed_in_range_check;
}

struct InRangeCheck * App_Wheels_GetRightWheelSpeedInRange(const struct Wheels * wheels){
    return wheels->right_wheel_speed_in_range_check;
}

void App_Wheels_Broadcast(struct FsmCanTxInterface *can_tx, const struct Wheels * wheels){
    struct InRangeCheck *left_wheel_speed_in_range_check  = App_Wheels_GetLeftWheelSpeedInRange(wheels);
    struct InRangeCheck *right_wheel_speed_in_range_check = App_Wheels_GetRightWheelSpeedInRange(wheels);

    App_SetPeriodicCanSignals_InRangeCheck(
            can_tx, left_wheel_speed_in_range_check, App_CanTx_SetPeriodicSignal_LEFT_WHEEL_SPEED,
            App_CanTx_SetPeriodicSignal_LEFT_WHEEL_SPEED_OUT_OF_RANGE,
            CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_FSM_NON_CRITICAL_ERRORS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
            can_tx, right_wheel_speed_in_range_check, App_CanTx_SetPeriodicSignal_RIGHT_WHEEL_SPEED,
            App_CanTx_SetPeriodicSignal_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
            CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OK_CHOICE,
            CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_UNDERFLOW_CHOICE,
            CANMSGS_FSM_NON_CRITICAL_ERRORS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
