#pragma once

struct WheelSpeedSensor;

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
    const struct WheelSpeedSensor *wheel_speed_sensor);
void App_WheelSpeedSensor_Tick(struct WheelSpeedSensor *wheel_speed_sensor);
