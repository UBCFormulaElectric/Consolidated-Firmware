#include "app_activeDifferential.hpp"
#include "app_vd_constants.hpp"
#include "app_units.h"

#include <cmath>

// Maximum motor speed limit for clamping (6000 RPM converted to rad/s)
// constexpr float MAX_MOTOR_SPEED_RPM   = 6000.0f;
// constexpr float MAX_MOTOR_SPEED_RAD_S = RPM_TO_RADS(MAX_MOTOR_SPEED_RPM);

// static inline float clamp_limit(float x, float limit)
//{
//     if (x > limit)
//         return limit;
//     if (x < -limit)
//         return -limit;
//     return x;
// }

/**
 * Converts steering wheel angle to wheel angle using approximation constant.
 * TODO: Replace with proper transfer function when available.
 *
 * @param steering_wheel_angle_deg Steering wheel angle in degrees
 * @return Wheel angle in radians
 */
static float steeringToWheelAngle(const float steering_wheel_angle_deg)
{
    // Placeholder function - will be replaced with proper transfer function
    // Currently uses approximation constant APPROX_STEERING_TO_WHEEL_ANGLE
    return static_cast<float>(
        DEG_TO_RAD(steering_wheel_angle_deg * static_cast<float>(APPROX_STEERING_TO_WHEEL_ANGLE)));
}
/**
 * Electronic Differential Speed Control for 4-wheel drive vehicle
 *
 * Implements the algorithm from "Electronic differential speed control for two in-wheels
 * motors drive vehicle" paper, adapted for 4WD system.
 *
 * Algorithm:
 * 1) Compute wheel speed difference Δω (Eq. 13): Δω = (TRACK_WIDTH_m/WHEELBASE_m) * tan(δ) * ω_v
 * 2) Split base speed into left/right wheel references (Eq. 15-16):
 *    ω_L_wheel = ω_v + 0.5 * Δω
 *    ω_R_wheel = ω_v - 0.5 * Δω
 * 3) Convert wheel refs to motor refs using gear ratio (Eq. 17-18):
 *    ω_L_motor = GEAR_RATIO * ω_L_wheel
 *    ω_R_motor = GEAR_RATIO * ω_R_wheel
 * 4) Allocate speeds to inner/outer wheels:
 *    - Left turn (δ > 0): Right wheels (FR, RR) are outer (faster),
 *                        Left wheels (FL, RL) are inner (slower)
 *    - Right turn (δ < 0): Right wheels (FR, RR) are inner (slower),
 *                         Left wheels (FL, RL) are outer (faster)
 *    - Straight (δ = 0): All wheels same speed
 * 5) Limit motor speeds for safety
 *
 * @param yaw_rate_rad_s Actual vehicle angular velocity (yaw rate) from IMU (rad/s)
 *                    Positive = counter-clockwise (left turn), Negative = clockwise (right turn)
 * @param steering_wheel_angle_deg Steering wheel angle in degrees (positive = left turn, negative = right turn)
 * @return delta omega suggested by this controller
 */
controllerTorqueDeltas
    app_activeDifferential_computeTorque(const float yaw_rate_rad_s, const float steering_wheel_angle_deg)
{
    // Convert steering wheel angle to wheel angle (delta)
    const float wheel_angle = steeringToWheelAngle(steering_wheel_angle_deg);

    // Step 1: Compute wheel speed difference Δω (Eq. 13 from paper)
    // Δω = (TRACK_WIDTH_m/WHEELBASE_m) * tan(δ) * ω_v
    // Note: For left turn (positive steering), right wheels are outer (faster)
    const float delta_omega = static_cast<float>(
        (static_cast<double>(TRACK_WIDTH_m) / static_cast<double>(WHEELBASE_m)) *
        tan(static_cast<double>(wheel_angle)) * static_cast<double>(yaw_rate_rad_s));

    // Step 2: Split base speed into left/right wheel references (Eq. 15-16 from paper)
    // Note: For left turn (positive steering), right wheels are outer (faster)
    // This matches positive yaw rate (counter-clockwise) corresponding to positive steering angle
    // const float omega_L_wheel = omega_v - 0.5f * delta_omega;
    // const float omega_R_wheel = omega_v + 0.5f * delta_omega;

    // Step 3: Convert wheel refs to motor refs using gear ratio (Eq. 17-18 from paper)
    // float omega_L_motor = GEAR_RATIO * omega_L_wheel;
    // float omega_R_motor = GEAR_RATIO * omega_R_wheel;

    // Step 4: Limit motor speeds for safety (clamp to prevent exceeding 6000 RPM limit)
    // Use provided omega_m_max if > 0, otherwise use default MAX_MOTOR_SPEED_RAD_S
    // const float max_speed_limit = (omega_m_max > 0.0f) ? omega_m_max : MAX_MOTOR_SPEED_RAD_S;

    // Clamp motor speeds to maximum allowed value
    // omega_L_motor = clamp_limit(omega_L_motor, max_speed_limit);
    // omega_R_motor = clamp_limit(omega_R_motor, max_speed_limit);

    // Assign motor references to individual wheels
    // const float omega_fl = omega_L_motor;
    // const float omega_rl = omega_L_motor;
    // const float omega_fr = omega_R_motor;
    // const float omega_rr = omega_R_motor;

    // Return delta_omega (wheel speed difference) in the struct; gearbox/splitting left to caller.
    controllerTorqueDeltas result;
    result.wheel_delta = delta_omega;
    return result;
}
