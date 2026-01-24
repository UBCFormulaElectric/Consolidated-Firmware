#include "app_activeDifferential.hpp"
#include "app_vd_constants.hpp"
#include "app_units.h"

#include <cmath>

// Maximum motor speed limit for clamping (6000 RPM converted to rad/s)
constexpr float MAX_MOTOR_SPEED_RPM   = 6000.0f;
constexpr float MAX_MOTOR_SPEED_RAD_S = RPM_TO_RADS(MAX_MOTOR_SPEED_RPM);

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

extern "C"
{
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
     *    - Right turn (δ > 0): Left wheels (FL, RL) are outer (faster),
     *                         Right wheels (FR, RR) are inner (slower)
     *    - Left turn (δ < 0): Left wheels (FL, RL) are inner (slower),
     *                         Right wheels (FR, RR) are outer (faster)
     *    - Straight (δ = 0): All wheels same speed
     * 5) Limit motor speeds for safety
     *
     * @param omega_v_ref Desired vehicle base angular speed (rad/s)
     * @param steering_wheel_angle_deg Steering wheel angle in degrees (positive = right turn, negative = left turn)
     * @param omega_m_max Maximum motor speed (rad/s) for saturation. If <= 0, no saturation applied.
     * @return Motor speed references in rad/s (inner and outer wheels allocated appropriately for 4WD)
     */
    TorqueAllocationOutputs app_activeDifferential_computeTorque(
        const float omega_v_ref,
        const float steering_wheel_angle_deg,
        const float omega_m_max)
    {
        // Convert steering wheel angle to wheel angle (delta)
        const float delta = steeringToWheelAngle(steering_wheel_angle_deg);

        // Step 1: Compute wheel speed difference Δω (Eq. 13 from paper)
        // Δω = (TRACK_WIDTH_m/WHEELBASE_m) * tan(δ) * ω_v
        const float delta_omega = (TRACK_WIDTH_m / WHEELBASE_m) * tanf(delta) * omega_v_ref;

        // Step 2: Split base speed into left/right wheel references (Eq. 15-16 from paper)
        // Note: The sign of delta_omega determines which side is inner/outer
        const float omega_L_wheel_ref = omega_v_ref + 0.5f * delta_omega;
        const float omega_R_wheel_ref = omega_v_ref - 0.5f * delta_omega;

        // Step 3: Convert wheel refs to motor refs using gear ratio (Eq. 17-18 from paper)
        float omega_L_motor_ref = GEAR_RATIO * omega_L_wheel_ref;
        float omega_R_motor_ref = GEAR_RATIO * omega_R_wheel_ref;

        // Step 4: Limit motor speeds for safety (clamp to prevent exceeding 6000 RPM limit)
        // Use provided omega_m_max if > 0, otherwise use default MAX_MOTOR_SPEED_RAD_S
        const float max_speed_limit = (omega_m_max > 0.0f) ? omega_m_max : MAX_MOTOR_SPEED_RAD_S;

        // Clamp motor speeds to maximum allowed value
        if (omega_L_motor_ref > max_speed_limit)
        {
            omega_L_motor_ref = max_speed_limit;
        }
        else if (omega_L_motor_ref < -max_speed_limit)
        {
            omega_L_motor_ref = -max_speed_limit;
        }

        if (omega_R_motor_ref > max_speed_limit)
        {
            omega_R_motor_ref = max_speed_limit;
        }
        else if (omega_R_motor_ref < -max_speed_limit)
        {
            omega_R_motor_ref = -max_speed_limit;
        }

        // Step 5: Allocate speeds to inner/outer wheels based on turn direction
        // For 4WD: inner wheels (both front and rear) get the same speed,
        //          outer wheels (both front and rear) get the same speed
        //
        // Turn direction logic:
        // - delta > 0 (right turn): delta_omega > 0, so omega_L > omega_R
        //   -> Left wheels are outer (faster), Right wheels are inner (slower)
        // - delta < 0 (left turn): delta_omega < 0, so omega_L < omega_R
        //   -> Left wheels are inner (slower), Right wheels are outer (faster)
        // - delta == 0 (straight): delta_omega == 0, so omega_L == omega_R
        //   -> All wheels same speed

        float omega_fl, omega_fr, omega_rl, omega_rr;

        if (delta > 0.0f)
        {
            // Right turn: right wheels (FR, RR) are inner (slower), left wheels (FL, RL) are outer (faster)
            omega_fl = omega_L_motor_ref; // Front left - outer (faster)
            omega_fr = omega_R_motor_ref; // Front right - inner (slower)
            omega_rl = omega_L_motor_ref; // Rear left - outer (faster)
            omega_rr = omega_R_motor_ref; // Rear right - inner (slower)
        }
        else if (delta < 0.0f)
        {
            // Left turn: left wheels (FL, RL) are inner (slower), right wheels (FR, RR) are outer (faster)
            omega_fl = omega_L_motor_ref; // Front left - inner (slower)
            omega_fr = omega_R_motor_ref; // Front right - outer (faster)
            omega_rl = omega_L_motor_ref; // Rear left - inner (slower)
            omega_rr = omega_R_motor_ref; // Rear right - outer (faster)
        }
        else
        {
            // Straight: all wheels same speed (omega_L == omega_R when delta_omega == 0)
            omega_fl = omega_L_motor_ref;
            omega_fr = omega_L_motor_ref;
            omega_rl = omega_L_motor_ref;
            omega_rr = omega_L_motor_ref;
        }

        // Return motor speeds allocated to inner/outer wheels
        // note: Currently returning in TorqueAllocationOutputs structure (using quintuna structure format)
        TorqueAllocationOutputs result;
        result.front_left_torque  = omega_fl; // Front left motor speed (rad/s)
        result.front_right_torque = omega_fr; // Front right motor speed (rad/s)
        result.rear_left_torque   = omega_rl; // Rear left motor speed (rad/s)
        result.rear_right_torque  = omega_rr; // Rear right motor speed (rad/s)
        return result;
    }
} // extern "C"
