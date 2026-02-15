#pragma once
#include "app_pid.hpp"

namespace app::tv::controllers::dyrc
{
class DirectYawRateControl
{
  private:
    // Configuration
    const PID::Config pid_config;
    const float       ku = 0.0f; // understeer gradient

    // Control Scheme
    PID pid;

    // purely for debugging purposes
    float yaw_moment_Nm = 0.0f;
    float r_ref_rad     = 0.0f;

  public:
    explicit DirectYawRateControl(const PID::Config &in_pid_conf, const float in_ku)
      : pid_config(in_pid_conf), ku(in_ku), pid(in_pid_conf){};

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
    [[nodiscard]] inline float computeRefYawRate(float steer_ang_rad, float body_velx_mps);

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
    [[nodiscard]] inline float computeYawMoment(float r_actual_rad, float steer_ang_rad, float body_velx_mps);

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
};
} // namespace app::tv::controllers::dyrc