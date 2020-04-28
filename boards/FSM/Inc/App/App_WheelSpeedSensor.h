#pragma once

struct WheelSpeedSensor;

enum WheelSpeedStatus
{
    WHEEL_SPEED_NORMAL,
    WHEEL_SPEED_NON_CRITICAL_FAULT
};

/**
 * Create the wheel speed sensor to read the wheel speed from
 * @param get_wheel_speed: A function pointer to a function that
 * acquires the wheel speed (km/h)from the wheel speed sensor
 * @return A pointer to the created wheel speed sensor, whose ownership is given
 * to the caller
 */
struct WheelSpeedSensor *
      App_WheelSpeedSensor_Create(float (*get_wheel_speed)(void));
void  App_WheelSpeedSensor_Destroy(struct WheelSpeedSensor *wheel_speed_sensor);
float App_WheelSpeedSensor_GetWheelSpeed(
    struct WheelSpeedSensor *wheel_speed_sensor);
enum WheelSpeedStatus
    App_WheelSpeedSensor_GetStatus(struct WheelSpeedSensor *wheel_speed_sensor);
