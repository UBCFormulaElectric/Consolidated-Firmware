#pragma once
#include "app_pid.hpp"

namespace app::tv::controllers::dyrc
{
/**
 * @brief Computes the reference yaw rate to target based on the steering angle, velocity and understeer gradient
 *
 * @param steer_ang_rad The current steering angle of the vehicle in radians
 * @param body_velx_mps The longitudinal velocity of the vehicle in m/s
 *
 * The reference yaw rate is computed using the formula:
 *
 * r_ref = (Vx * delta) / (L * ( + ku * Vx^2))
 *
 * Where:
 * - Vx is the longitudinal velocity of the vehicle in m/s
 * - delta is the steering angle in radians
 * - L is the wheelbase of the vehicle in meters
 * - ku is the understeer gradient in 1/m/s^2
 *
 * @return The reference yaw rate to target in rad/s
 */
[[nodiscard]] inline float computeRefYawRate(const float steer_ang_rad, const float body_velx_mps);

/**
 * @brief Computes the corrective yaw moment to apply on the vehicle to target a reference yaw rate
 * using a PID controller. The reference yaw rate is computed using the understeer gradient, steering angle and
 * velocity.
 *
 * @param r_actual_rad The current yaw rate of the vehicle
 * @param steer_ang_rad The current steering angle of the vehicle in radians
 * @param body_velx_mps The longitudinal velocity of the vehicle in m/s
 *
 * @return The corrective yaw moment in Nm to apply on the vehicle
 */
[[nodiscard]] inline float
    computeYawMoment(const float r_actual_rad, const float steer_ang_rad, const float body_velx_mps);

// The functions below are getters for CAN debugging

/**
 * @brief Get the corrective yaw moment computed by the PID controller
 *
 * @return The corrective yaw moment in Nm to apply on the vehicle
 */
[[nodiscard]] float getYawMoment();

/**
 * @brief Get the reference yaw rate computed using the understeer gradient, steering angle and velocity
 *
 * @return The desired yaw rate to achieve in radians per second
 */
[[nodiscard]] float getRefYawRate();
} // namespace app::tv::controllers::dyrc