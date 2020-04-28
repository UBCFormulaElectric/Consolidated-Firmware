#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <math.h>
#include "App_WheelSpeedSensor.h"

struct WheelSpeedSensor
{
    float wheel_speed;
    float (*get_wheel_speed)(void);

    enum WheelSpeedFaultStatus status;
};

static enum WheelSpeedFaultStatus
    App_WheelSpeedSensor_CheckStatus(float wheel_speed);

struct WheelSpeedSensor *
    App_WheelSpeedSensor_Create(float (*get_wheel_speed)(void))
{
    assert(get_wheel_speed != NULL);
    struct WheelSpeedSensor *wheel_speed_sensor =
        (struct WheelSpeedSensor *)malloc(sizeof(struct WheelSpeedSensor));

    assert(wheel_speed_sensor != NULL);

    wheel_speed_sensor->wheel_speed     = NAN;
    wheel_speed_sensor->get_wheel_speed = get_wheel_speed;
    wheel_speed_sensor->status          = WHEEL_SPEED_NORMAL;

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
    wheel_speed_sensor->wheel_speed = wheel_speed_sensor->get_wheel_speed();
    return wheel_speed_sensor->wheel_speed;
}

static enum WheelSpeedFaultStatus
    App_WheelSpeedSensor_CheckStatus(const float wheel_speed)
{
    // Send a non critical fault when the wheel speed is above 150 km/h OR when
    // the wheel speed sensor is inactive
    return (isnanf(wheel_speed) || wheel_speed > 150.0f)
               ? WHEEL_SPEED_NON_CRITICAL_FAULT
               : WHEEL_SPEED_NORMAL;
}

enum WheelSpeedFaultStatus App_WheelSpeedSensor_GetFaultStatus(
    struct WheelSpeedSensor *const wheel_speed_sensor)
{
    wheel_speed_sensor->status =
        App_WheelSpeedSensor_CheckStatus(wheel_speed_sensor->wheel_speed);
    return wheel_speed_sensor->status;
}
