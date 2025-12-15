#pragma once
#include "app_units.h"

/**
 * @brief Computes the effective rolling tire radius for a wheel
 *
 * The effective rolling radius is calculated using the equation:
 * re = Vx / ω
 *
 * Where:
 * - Vx is the wheel velocity in the x-direction (wheel frame)
 * - ω is the angular velocity (rad/s) = 2πf, where f is frequency from IMU
 *
 * The wheel velocity Vx is computed from body frame velocities:
 * V_x,i^wheel = V_x,i^body cos(δ_i) + V_y,i^body sin(δ_i)
 *
 * @param v_x_body Body frame velocity in x-direction (m/s)
 * @param v_y_body Body frame velocity in y-direction (m/s)
 * @param steer_angle_deg Steering angle for the wheel in degrees
 * @param frequency_hz Frequency from IMU in Hz
 * @return Effective rolling tire radius in meters, or 0.0f if frequency is invalid
 */
float app_effectiveRollingRadius_compute(float v_x_body, float v_y_body, float steer_angle_deg, float frequency_hz);




