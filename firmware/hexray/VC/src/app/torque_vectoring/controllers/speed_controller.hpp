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
 *
 * for tire ffb calculation
 * @param F_x_N longitudinal force
 * @param radial_deflection_m the vertical compression of a tire under vehicle load, measured as difference between
 * unloaded and loaded height
 * @param M_y_Nm the calculated rolling resistance moment from tire model
 *
 * @param torque_max_Nm / torque_min_Nm  max and min torque limits in Nm
 *
 * @return per wheel torque request in Nm
 *
 * */
template <Decimal T>
[[nodiscard]] shared_datatypes::wheel_set<T> computeTorque(
    const shared_datatypes::wheel_set<T> &speed_request_radps,
    const shared_datatypes::wheel_set<T> &omega_radps,
    const shared_datatypes::wheel_set<T> &F_x_N,
    const shared_datatypes::wheel_set<T> &radial_deflection_m,
    const shared_datatypes::wheel_set<T> &M_y_Nm,
    const shared_datatypes::wheel_set<T> &torque_max_Nm,
    const shared_datatypes::wheel_set<T> &torque_min_Nm);

void reset();
} // namespace app::tv::controllers::speed_controller