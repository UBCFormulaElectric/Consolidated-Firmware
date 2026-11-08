#pragma once
#include "app_pid.hpp"
#include "controls/datatypes/wheel_set.hpp"

namespace app::controls::actuation::speed_controller
{
// The car counts as stopped (speed controllers reset) while the estimated |v_x| and |v_y| are both below this. The
// estimate is never exactly 0; at standstill the zero-speed update holds it within a few mm/s.
static constexpr float STOPPED_SPEED_MPS = 0.05f;

// actual velocity in INVRL is the motor rpm so no need to multiply by gear ratio.
/**
 * @brief Computes the motor torques that track the motor speed setpoints, with a PID per wheel and the tire force
 * feedforward.
 *
 * @param motor_speed_request_radps motor speed request setpoints from kappa_update in rad/s
 * @param motor_omega_radps  measured motor speeds in rad/s
 *
 * for tire ffb calculation
 * @param fx_N longitudinal tire force in N
 * @param radial_deflection_m the vertical compression of a tire under vehicle load, measured as difference between
 * unloaded and loaded height
 * @param my_Nm the calculated rolling resistance moment from tire model in Nm
 *
 * @param torque_max_Nm / torque_min_Nm  max and min motor torque limits in Nm
 *
 * @return per wheel motor torque request in Nm
 *
 * */
template <Decimal T>
[[nodiscard]] wheel_set<T> computeTorque(
    const wheel_set<T> &motor_speed_request_radps,
    const wheel_set<T> &motor_omega_radps,
    const wheel_set<T> &fx_N,
    const wheel_set<T> &radial_deflection_m,
    const wheel_set<T> &my_Nm,
    const wheel_set<T> &torque_max_Nm,
    const wheel_set<T> &torque_min_Nm);

void reset();
} // namespace app::controls::actuation::speed_controller