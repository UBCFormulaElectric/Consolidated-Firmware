#include <stdlib.h>
#include <assert.h>
#include "App_Steering.h"
#include "configs/App_SteeringAngleThresholds.h"

#include "App_InRangeCheck.h"
#include "App_SharedSetPeriodicCanSignals.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

struct Steering
{
    float (*get_steering_angle)(void);
    struct InRangeCheck *steering_angle_in_range_check;
};

struct Steering *App_Steering_Create(float (*get_steering_angle)(void))
{
    struct Steering *steering = malloc(sizeof(struct Steering));
    assert(steering != NULL);

    steering->get_steering_angle = get_steering_angle;

    steering->steering_angle_in_range_check =
        App_InRangeCheck_Create(get_steering_angle, MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG);
    return steering;
}

struct InRangeCheck *App_Steering_GetInRange(const struct Steering *steering)
{
    return steering->steering_angle_in_range_check;
}

void App_Steering_Broadcast(struct FsmCanTxInterface *can_tx, const struct Steering *steering)
{
    struct InRangeCheck *steering_angle_in_range_check = App_Steering_GetInRange(steering);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, steering_angle_in_range_check, App_CanTx_SetPeriodicSignal_STEERING_ANGLE,
        App_CanTx_SetPeriodicSignal_STEERING_ANGLE_OUT_OF_RANGE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
