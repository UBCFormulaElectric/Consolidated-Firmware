#pragma once

#include "controls/datatypes/wheel_set.hpp"
#include "controls/params.hpp"
#include "controls/datatypes/decimal_dual.hpp"

namespace app::controls::actuation::power_limit
{

/**
* @brief Compute the de-rated torque requests for each motor given power limit
*
* @param motor_omega_radps measured motor speeds in rad/s
* @param motor_torque_Nm the desired motor torques from the speed controller in Nm
* @param torque_max_Nm / torque_min_Nm per-motor torque limits in Nm
* @param discharge_limit_W power limit when discharging from the battery pack in W, from the BMS
* @param charge_limit_W power limit when charging the battery pack (regen) in W, from the BMS

* @return De-rated motor torques in Nm
**/

template <Decimal T>
wheel_set<T> limitPower(
    wheel_set<T> motor_omega_radps,
    wheel_set<T> motor_torque_Nm,
    wheel_set<T> torque_max_Nm,
    wheel_set<T> torque_min_Nm,
    T            discharge_limit_W,
    T            charge_limit_W);

} // namespace app::controls::actuation::power_limit