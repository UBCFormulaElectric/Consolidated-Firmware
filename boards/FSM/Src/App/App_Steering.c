#include <stdlib.h>
#include <assert.h>
#include "App_Steering.h"
#include "configs/App_SteeringAngleThresholds.h"

#include "App_InRangeCheck.h"
#include "App_SharedSetPeriodicCanSignals.h"

// STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(FsmCanTxInterface)

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

    // set value TODO: JSONCAN ->
    // App_CanTx_SetPeriodicSignal_STEERING_ANGLE(can_tx, steering->get_steering_angle());

    bool steering_sensor_ocsc = steering->steering_sensor_OCSC();
    // TODO: JSONCAN ->
    // uint8_t CANMSGS_FSM_STEERING_ANGLE_SENSOR_STEERING_SENSOR_OCSC =
    //    steering_sensor_ocsc ? CANMSGS_FSM_STEERING_ANGLE_SENSOR_STEERING_SENSOR_OCSC_TRUE_CHOICE
    //                         : CANMSGS_FSM_STEERING_ANGLE_SENSOR_STEERING_SENSOR_OCSC_FALSE_CHOICE;
    // App_CanTx_SetPeriodicSignal_STEERING_SENSOR_OCSC(can_tx, CANMSGS_FSM_STEERING_ANGLE_SENSOR_STEERING_SENSOR_OCSC);
    if (steering_sensor_ocsc)
    {
        // App_CanTx_SetPeriodicSignal_STEERING_ANGLE(can_tx, 0);
    }

    struct InRangeCheck *steering_angle_in_range_check = steering->steering_angle_in_range_check;
    // App_SetPeriodicCanSignals_InRangeCheck(
    //    can_tx, steering_angle_in_range_check, App_CanTx_SetPeriodicSignal_STEERING_ANGLE,
    //    App_CanTx_SetPeriodicSignal_STEERING_ANGLE_OUT_OF_RANGE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_OK_CHOICE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_WIDELEFT_CHOICE,
    //    CANMSGS_FSM_NON_CRITICAL_ERRORS_STEERING_ANGLE_OUT_OF_RANGE_WIDERIGHT_CHOICE);
}
