#pragma once

/**
 * Create the wheel speed sensor to read the wheel speed from
 * @param get_wheel_speed: A function pointer to a function that
 * acquires the wheel speed (km/h)from the wheel speed sensor
 * @return A pointer to the created wheel speed sensor, whose ownership is given
 * to the caller
 */
struct WheelSpeedSensor *
    App_WheelSpeedSensor_Create(float (*get_wheel_speed)(void));

/**
 * Destroying the wheel speed sensor object freeing any memory that it uses
 * @param wheel_speed_sensor The wheel speed sensor to destroy
 */
void App_WheelSpeedSensor_Destroy(struct WheelSpeedSensor *wheel_speed_sensor);

/**
 * Get the wheel speed in km/h from a wheel speed sensor
 * @param wheel_speed_sensor The wheel speed sensor to acquire and update the
 * wheel speed from
 * @return The wheel speed in km/h
 */
float App_WheelSpeedSensor_GetWheelSpeed(
    struct WheelSpeedSensor *wheel_speed_sensor);
