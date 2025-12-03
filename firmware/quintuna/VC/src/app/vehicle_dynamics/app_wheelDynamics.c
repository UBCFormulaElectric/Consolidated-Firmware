#include "app_wheelDynamics.h"
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
#include <assert.h>
#include <math.h>

float app_wheelDynamics_calculateEffectiveRollingRadius(
    const float wheel_velocity_mps,
    const float angular_velocity_rads)
{
    // Avoid division by zero
    if (fabsf(angular_velocity_rads) < SMALL_EPSILON)
    {
        return 0.0f;
    }

    // Core equation: re = Vx / ω
    return wheel_velocity_mps / angular_velocity_rads;
}

float app_wheelDynamics_calculateEffectiveRollingRadiusFromFrequency(
    const float wheel_velocity_mps,
    const float frequency_hz)
{
    // Avoid division by zero
    if (fabsf(frequency_hz) < SMALL_EPSILON)
    {
        return 0.0f;
    }

    // Convert frequency to angular velocity: ω = 2πf
    const float angular_velocity_rads = 2.0f * M_PI_F * frequency_hz;

    // Calculate effective rolling radius: re = Vx / ω
    return app_wheelDynamics_calculateEffectiveRollingRadius(wheel_velocity_mps, angular_velocity_rads);
}

float app_wheelDynamics_calculateEffectiveRollingRadiusFromMotorRPM(
    const float motor_rpm,
    const float wheel_velocity_mps)
{
    // Avoid division by zero
    if (fabsf(motor_rpm) < SMALL_EPSILON)
    {
        return 0.0f;
    }

    // Convert motor RPM to wheel angular velocity accounting for gear ratio
    // Motor RPM -> rad/s, then divide by gear ratio to get wheel angular velocity
    const float motor_angular_velocity_rads = RPM_TO_RADS(motor_rpm);
    const float wheel_angular_velocity_rads = motor_angular_velocity_rads / GEAR_RATIO;

    // Calculate effective rolling radius: re = Vx / ω
    return app_wheelDynamics_calculateEffectiveRollingRadius(wheel_velocity_mps, wheel_angular_velocity_rads);
}

float app_wheelDynamics_calculateWheelVelocityFromBodyFrame(
    const float body_velocity_x_mps,
    const float body_velocity_y_mps,
    const float steer_angle_rad)
{
    // Equation: V_x,i^wheel = V_x,i^body cos(δ_i) + V_y,i^body sin(δ_i)
    return body_velocity_x_mps * cosf(steer_angle_rad) + body_velocity_y_mps * sinf(steer_angle_rad);
}

