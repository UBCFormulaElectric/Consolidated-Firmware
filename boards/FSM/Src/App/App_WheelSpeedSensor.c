#include <stdlib.h>
#include <assert.h>

struct WheelSpeedSensor
{
    float (*get_wheel_speed)(void);
};

struct WheelSpeedSensor *
    App_WheelSpeedSensor_Create(float (*get_wheel_speed)(void))
{
    assert(get_wheel_speed != NULL);
    struct WheelSpeedSensor *wheel_speed_sensor =
        (struct WheelSpeedSensor *)malloc(sizeof(struct WheelSpeedSensor));

    assert(wheel_speed_sensor != NULL);

    wheel_speed_sensor->get_wheel_speed = get_wheel_speed;
    return wheel_speed_sensor;
}

void App_WheelSpeedSensor_Destroy(
    struct WheelSpeedSensor *const wheel_speed_sensor)
{
    free(wheel_speed_sensor);
}

float App_WheelSpeedSensor_GetWheelSpeed(
    struct WheelSpeedSensor *const wheel_speed_sensor)
{
    return wheel_speed_sensor->get_wheel_speed();
}
