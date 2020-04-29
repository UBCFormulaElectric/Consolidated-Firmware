#include <stdlib.h>
#include <assert.h>
#include "App_Wheel.h"

struct WheelSpeedSensor
{
    float (*get_wheel_speed)(void);
    float wheel_speed_threshold;
};

struct WheelSpeedSensor *App_Wheel_Create(
    float (*get_wheel_speed)(void),
    const float wheel_speed_threshold)
{
    assert(get_wheel_speed != NULL);
    struct WheelSpeedSensor *wheel_speed_sensor =
        (struct WheelSpeedSensor *)malloc(sizeof(struct WheelSpeedSensor));

    assert(wheel_speed_sensor != NULL);

    wheel_speed_sensor->get_wheel_speed       = get_wheel_speed;
    wheel_speed_sensor->wheel_speed_threshold = wheel_speed_threshold;
    return wheel_speed_sensor;
}

void App_Wheel_Destroy(struct WheelSpeedSensor *const wheel_speed_sensor)
{
    free(wheel_speed_sensor);
}

float App_Wheel_GetWheelSpeed(struct WheelSpeedSensor *const wheel_speed_sensor)
{
    return wheel_speed_sensor->get_wheel_speed();
}

float App_Wheel_GetThreshold(
    const struct WheelSpeedSensor *const wheel_speed_sensor)
{
    return wheel_speed_sensor->wheel_speed_threshold;
}
