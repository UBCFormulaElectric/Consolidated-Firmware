#include "power_limiting.hpp"
#include "torque_vectoring/tv_debug.h"

namespace app::tv::controllers::power_limiting
{

using Wheel = app::tv::shared_datatypes::Wheel;

template <Decimal T>
shared_datatypes::wheel_set<T> limitPower(
    shared_datatypes::wheel_set<T> desired_motor_omegas,
    shared_datatypes::wheel_set<T> desired_motor_torques,
    T                              discharge_pl,
    T                              charge_pl)
{
    T sum_omegas         = 0;
    T sum_torques        = 0;
    T desired_power_draw = 0;

    for (const Wheel wheel : { Wheel::FL, Wheel::FR, Wheel::RL, Wheel::RR })
    {
        sum_omegas += desired_motor_omegas[wheel];
        sum_torques += desired_motor_torques[wheel];

        desired_power_draw += desired_motor_omegas[wheel] * desired_motor_torques[wheel];
    }

    T delta_torque;

    if (desired_power_draw < -1 * charge_pl)
    {
        delta_torque = (desired_power_draw + charge_pl) / sum_omegas;
    }
    else if (desired_power_draw > discharge_pl)
    {
        delta_torque = (desired_power_draw - discharge_pl) / sum_omegas;
    }
    else
    {
        delta_torque = 0;
    }

    tv_debug_data.plim = { .desired_power_draw = desired_power_draw,
                           .discharge_pl       = discharge_pl,
                           .charge_pl          = charge_pl };

    return {
        .fl = desired_motor_torques.fl - delta_torque,
        .fr = desired_motor_torques.fr - delta_torque,
        .rl = desired_motor_torques.rl - delta_torque,
        .rr = desired_motor_torques.rr - delta_torque,

    };
}

// don't forget to make an explicit instatiation request so the compiler compiles your function in the .lib file!
template shared_datatypes::wheel_set<float>
    limitPower<float>(shared_datatypes::wheel_set<float>, shared_datatypes::wheel_set<float>, float, float);

} // namespace app::tv::controllers::power_limiting