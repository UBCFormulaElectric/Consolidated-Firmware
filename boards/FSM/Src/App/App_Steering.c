#include <stdlib.h>
#include <assert.h>
#include "App_Steering.h"
#include "configs/App_SteeringAngleThresholds.h"

#include "App_InRangeCheck.h"
#include "App_SharedSetPeriodicCanSignals.h"

// STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK

struct Steering
{
    float (*get_steering_angle)(void);
    bool (*steering_sensor_OCSC)(void);
    struct InRangeCheck *steering_angle_in_range_check;
};

struct Steering *App_Steering_Create(float (*get_steering_angle)(void), bool (*steering_sensor_OCSC)(void))
{
    struct Steering *steering = malloc(sizeof(struct Steering));
    assert(steering != NULL);

    steering->get_steering_angle   = get_steering_angle;
    steering->steering_sensor_OCSC = steering_sensor_OCSC;

    steering->steering_angle_in_range_check =
        App_InRangeCheck_Create(get_steering_angle, MIN_STEERING_ANGLE_DEG, MAX_STEERING_ANGLE_DEG);
    return steering;
}
void App_Steering_Destroy(struct Steering *steering)
{
    App_InRangeCheck_Destroy(steering->steering_angle_in_range_check);
    free(steering);
}

void App_Steering_Broadcast(const struct FsmWorld *world)
{
    struct Steering *steering = App_FsmWorld_GetSteering(world);

    App_CanTx_FSM_Steering_SteeringAngle_Set(steering->get_steering_angle());

    bool    steering_sensor_ocsc = steering->steering_sensor_OCSC();
    uint8_t SteeringAngleOCSCEnum =
        steering_sensor_ocsc ? STEERING_SENSOR_OCSC_TRUE_CHOICE : STEERING_SENSOR_OCSC_FALSE_CHOICE;
    App_CanTx_FSM_Errors_SteeringAngleSensorOCSC_Set(SteeringAngleOCSCEnum);
    if (steering_sensor_ocsc)
        App_CanTx_FSM_Steering_SteeringAngle_Set(0);

    struct InRangeCheck *steering_angle_in_range_check = steering->steering_angle_in_range_check;
    App_SetPeriodicCanSignals_InRangeCheck_float(
        steering_angle_in_range_check, App_CanTx_FSM_Steering_SteeringAngle_Set,
        (void (*)(uint8_t))App_CanTx_FSM_Errors_SteeringAngleOutOfRange_Set, STEERING_ANGLE_OUT_OF_RANGE_OK_CHOICE,
        STEERING_ANGLE_OUT_OF_RANGE_WIDELEFT_CHOICE, STEERING_ANGLE_OUT_OF_RANGE_WIDERIGHT_CHOICE);
}
