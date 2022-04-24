#pragma once
#include <stdbool.h>

#define MAX_TORQUE_REQUEST_NM 50.0f
#define REGEN_WHEEL_SPEED_THRESHOLD_KPH 5.0f

/**
 * Calculate torque request to both inverters
 * @param right_wheel_speed_kph Right wheel speed in km/h
 * @param left_wheel_speed_kph  Left wheel speed in km/h
 * @param air_negative_status   Status of AIR- (true = closed, false = open)
 * @param air_positive_status   Status of AIR+ (true = closed, false = open)
 * @param regen_paddle_percentage Regen paddle percentage
 * @param accel_pedal_percentage  Acceleration pedal percentage
 * @return
 */
float App_SharedTorqueRequest_CalculateTorqueRequest(
    float right_wheel_speed_kph,
    float left_wheel_speed_kph,
    bool  air_negative_status,
    bool  air_positive_status,
    float regen_paddle_percentage,
    float accel_pedal_percentage);
