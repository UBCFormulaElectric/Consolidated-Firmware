#pragma once

#include "torque_vectoring/shared_datatypes/wheel_set.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/decimal_dual.hpp"

namespace app::tv::controllers::power_limiting
{

/**
* @brief Compute the de-rated torque requests for each motor given power limit
*
* @param desired_omegas vector of current wheel speeds
* @param desired_torques the desired torques from the speed controller
* @param discharge_pl power limit when discharging from the battery pack, from the BMS
* @param charge_pl power limit when charging the battery pack (regen)

* @return De-rated torques
**/

template <Decimal T>
shared_datatypes::wheel_set<T> limitPower(
    shared_datatypes::wheel_set<T> desired_omegas,
    shared_datatypes::wheel_set<T> desired_torques,
    T                              discharge_pl,
    T                              charge_pl);

} // namespace app::tv::controllers::power_limiting