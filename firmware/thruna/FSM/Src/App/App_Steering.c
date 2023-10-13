#include <stdlib.h>
#include <assert.h>
#include "App_Steering.h"
#include "configs/App_SteeringAngleThresholds.h"

#include "App_InRangeCheck.h"
#include "App_SharedSetPeriodicCanSignals.h"

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

    App_CanTx_FSM_SteeringAngle_Set(steering->get_steering_angle());

    bool steering_sensor_ocsc = steering->steering_sensor_OCSC();
    App_CanAlerts_SetWarning(SteeringAngleOCSCWarning, steering_sensor_ocsc);
    if (steering_sensor_ocsc)
    {
        App_CanTx_FSM_SteeringAngle_Set(0);
    }

    float                    steering_angle;
    enum InRangeCheck_Status steering_in_range =
        App_InRangeCheck_GetValue(steering->steering_angle_in_range_check, &steering_angle);
    App_CanTx_FSM_SteeringAngle_Set(steering_angle);
    App_CanAlerts_SetWarning(SteeringAngleOutOfRangeWarning, steering_in_range != VALUE_IN_RANGE);
}
