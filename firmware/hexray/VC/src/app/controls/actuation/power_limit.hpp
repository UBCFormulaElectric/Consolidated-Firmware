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

wheel_set<float> limitPower(
    wheel_set<float> motor_omega_radps,
    wheel_set<float> motor_torque_Nm,
    wheel_set<float> torque_max_Nm,
    wheel_set<float> torque_min_Nm,
    float            discharge_limit_W,
    float            charge_limit_W);

} // namespace app::controls::actuation::power_limit