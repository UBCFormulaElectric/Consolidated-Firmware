#pragma once
#include "app_pid.hpp"

namespace app::controls::reference::yaw_rate
{
/**
 * @brief Computes the reference yaw rate to target based on the steering angle, velocity and understeer gradient
 *
 * @param wheel_angle_rad The current road wheel steer angle in radians (not the steering wheel angle)
 * @param v_x_mps The longitudinal velocity of the vehicle in m/s
 *
 * The reference yaw rate is computed using the formula:
 *
 * r_ref = (Vx * delta) / (L * ( + ku * Vx^2))
 *
 * Where:
 * - Vx is the longitudinal velocity of the vehicle in m/s
 * - delta is the steering angle in radians
 * - L is the wheelbase of the vehicle in meters
 * - ku is the understeer gradient in s^2/m^2
 *
 * @return The reference yaw rate to target in rad/s
 */
[[nodiscard]] float computeRefYawRate(const float wheel_angle_rad, const float v_x_mps);

/**
 * @brief Computes the corrective yaw moment to apply on the vehicle to target a reference yaw rate
 * using a PID controller. The reference yaw rate is computed using the understeer gradient, steering angle and
 * velocity.
 *
 * @param yaw_rate_radps The current yaw rate of the vehicle in rad/s
 * @param wheel_angle_rad The current road wheel steer angle in radians
 * @param v_x_mps The longitudinal velocity of the vehicle in m/s
 *
 * @return The corrective yaw moment in Nm to apply on the vehicle
 */
[[nodiscard]] float computeYawMoment(const float yaw_rate_radps, const float wheel_angle_rad, const float v_x_mps);

} // namespace app::controls::reference::yaw_rate
