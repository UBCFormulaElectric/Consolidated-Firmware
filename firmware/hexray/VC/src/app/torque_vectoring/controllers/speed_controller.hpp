#pragma once
#include "app_pid.hpp"
#include "torque_vectoring/shared_datatypes/wheel_set.hpp"

namespace app::tv::controllers::speed_controller
{
// actual velocity in INVRL is the motor rpm so no need to multiply by gear ratio.
/**
 * @brief Computes the reference velocity to target based on the actual velocity, desired velocity, and PID controller.
 *
 * @param speed_request_radps motor speed request setpoints from kappa_update in rad/s
 * @param omega_radps  measured motor speeds in rad/s
 * @param tire_ffb_Nm  feedfordward torque from tire model in Nm
 * @param torque_max_Nm / torque_min_Nm  max and min torque limits in Nm
 *
 * @return per wheel torque request in Nm
 *
 * */
[[nodiscard]] shared_datatypes::wheel_set<float> computeTorque(
    const shared_datatypes::wheel_set<float> &speed_request_radps,
    const shared_datatypes::wheel_set<float> &omega_radps,
    const shared_datatypes::wheel_set<float> &tire_ffb_Nm,
    const shared_datatypes::wheel_set<float> &torque_max_Nm,
    const shared_datatypes::wheel_set<float> &torque_min_Nm);

void reset();
} // namespace app::tv::controllers::speed_controller