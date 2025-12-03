#pragma once
#include "app_vehicleDynamicsConstants.h"

/**
 * Calculate the effective rolling tire radius from wheel velocity and angular velocity.
 *
 * Core equation: re = Vx / ω
 * where:
 *   re = effective rolling tire radius (m)
 *   Vx = wheel velocity in wheel frame (m/s)
 *   ω  = wheel angular velocity (rad/s)
 *
 * @param wheel_velocity_mps Wheel velocity in the wheel frame (m/s)
 * @param angular_velocity_rads Wheel angular velocity (rad/s)
 * @return Effective rolling tire radius in meters, or 0.0f if angular_velocity is too small
 */
float app_wheelDynamics_calculateEffectiveRollingRadius(
    const float wheel_velocity_mps,
    const float angular_velocity_rads);

/**
 * Calculate the effective rolling tire radius from wheel velocity and frequency.
 *
 * This is a convenience function that converts frequency to angular velocity:
 * ω = 2πf
 * re = Vx / ω
 *
 * @param wheel_velocity_mps Wheel velocity in the wheel frame (m/s)
 * @param frequency_hz Wheel rotation frequency from sensor (Hz)
 * @return Effective rolling tire radius in meters, or 0.0f if frequency is too small
 */
float app_wheelDynamics_calculateEffectiveRollingRadiusFromFrequency(
    const float wheel_velocity_mps,
    const float frequency_hz);

/**
 * Calculate the effective rolling tire radius from motor RPM.
 *
 * This function converts motor RPM to wheel angular velocity accounting for gear ratio,
 * then calculates the effective rolling radius.
 *
 * @param motor_rpm Motor RPM
 * @param wheel_velocity_mps Wheel velocity in the wheel frame (m/s)
 * @return Effective rolling tire radius in meters, or 0.0f if motor_rpm is too small
 */
float app_wheelDynamics_calculateEffectiveRollingRadiusFromMotorRPM(
    const float motor_rpm,
    const float wheel_velocity_mps);

/**
 * Calculate wheel velocity in wheel frame from body frame velocities and steer angle.
 *
 * Equation: V_x,i^wheel = V_x,i^body cos(δ_i) + V_y,i^body sin(δ_i)
 * where:
 *   V_x,i^wheel = wheel velocity in wheel frame (m/s)
 *   V_x,i^body  = body frame velocity in x direction (m/s)
 *   V_y,i^body  = body frame velocity in y direction (m/s)
 *   δ_i         = steer angle for wheel i (rad)
 *
 * @param body_velocity_x_mps Body frame velocity in x direction (m/s)
 * @param body_velocity_y_mps Body frame velocity in y direction (m/s)
 * @param steer_angle_rad Steer angle in radians
 * @return Wheel velocity in wheel frame (m/s)
 */
float app_wheelDynamics_calculateWheelVelocityFromBodyFrame(
    const float body_velocity_x_mps,
    const float body_velocity_y_mps,
    const float steer_angle_rad);

