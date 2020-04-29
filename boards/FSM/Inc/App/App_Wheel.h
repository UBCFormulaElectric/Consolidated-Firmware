#pragma once
#include <math.h>

/**
 * Create the wheel speed sensor to read the wheel speed from
 * @param get_wheel_speed: A function pointer to a function that
 * acquires the wheel speed (km/h)from the wheel speed sensor
 * @return A pointer to the created wheel speed sensor, whose ownership is given
 * to the caller
 */
struct WheelSpeedSensor *App_Wheel_Create(
    float (*get_wheel_speed)(void),
    float wheel_speed_threshold);

/**
 * Destroying the wheel speed sensor object freeing any memory that it uses
 * @param wheel_speed_sensor The wheel speed sensor to destroy
 */
void App_Wheel_Destroy(struct WheelSpeedSensor *wheel_speed_sensor);

/**
 * Get the wheel speed in km/h from a wheel speed sensor
 * @param wheel_speed_sensor The wheel speed sensor to acquire and update the
 * wheel speed from
 * @return The wheel speed in km/h
 */
float App_Wheel_GetWheelSpeed(struct WheelSpeedSensor *wheel_speed_sensor);

/**
 * Get the upper wheel speed threshold (km/h) from a wheel speed sensor
 * @param wheel_speed_sensor The wheel speed sensor to acquire the threshold
 * from
 * @return The upper wheel speed threshold (km/h) of the given wheel speed
 * sensor
 */
float App_Wheel_GetThreshold(const struct WheelSpeedSensor *wheel_speed_sensor);
